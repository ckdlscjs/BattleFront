# 🎮 TheBattleFront 핵심 기능 및 소스코드 상세 분석

본 문서는 Unreal Engine 5.1 및 C++를 기반으로 개발된 3D 쿼터뷰 멀티플레이어 아레나 액션 게임 **'TheBattleFront'**의 핵심 AI/자기장 서버 데이터 클라이언트 중개 연동부, 서버 피드백 AIController, 건물/수풀 컬링 공간 제어 및 시네마틱 회전 연출 카메라 기능들을 실제 소스코드의 데이터 연산 흐름을 바탕으로 상세하게 분석한 기술 명세서임.

---

## 1. 서버 수신 데이터의 AI 및 맵 오브젝트 클라이언트 연동
* **파일명**: 
  - [Source/AI_MAP/Team_AI/Private/Team_AIGameMode.cpp](https://github.com/ckdlscjs/BattleFront/blob/main/Source/AI_MAP/Team_AI/Private/Team_AIGameMode.cpp) (SetAIMove 및 SetMagneticField 등 AI/MAP 데이터 중개 연동부)
* **기능 개요**: Dedicated Server의 브로드캐스트 세션에서 파싱 완료되어 수신된 몬스터의 위치/이동 패킷 및 자기장 수축 속성 패킷을 게임모드 제어기(GameMode) 내의 동기화 패스를 통해 몬스터(AI) 프록시 개체 및 자기장(MAP) 장막 물리체에 주입하여 클라이언트 상의 연동 일관성을 유지함.
* **코드 상세 분석**:
  - `SetAIMove()`는 서버로부터 수신된 몬스터의 위치 정보 패킷(`S_AIMOVE`) 내 고유 ObjectId를 활용해 `Enemies` 맵에서 해당 몬스터 프록시 개체를 식별한 뒤, 수신된 목표 좌표(`loc`)와 회전값(`rot`), 속도(`speed`)를 `RecvMoveTo()` 인터페이스에 대입함으로써 프록시 개체의 보간(Lerp) 이동 무브를 런타임 가동함.
  - `SetAIMoveStop()`은 몬스터의 정지 신호를 갱신하고, `SetAIDamaged()` 및 `SetAIDead()`는 몬스터의 피격 피해 및 사망 이벤트를 게임모드 컨테이너 내의 몬스터 객체에 각각 주입해 실시간 게임 상태를 통제함.
  - `SetMagneticField()`는 서버에서 송신된 안전지대 페이즈 데이터 패킷(`S_SET_MAGNETICFIELD`)을 해석해 색상 파라미터(`rgba`), 수축 반경(`radius`), 제한 시간(`time`)을 도출한 후, `ATeam_AIMagneticField` 클래스의 `RecvUpdateSafetyFieldValue()`를 호출해 맵 장막의 물리적 면적과 시각적 경고 렌더링 범위를 동기화함.

```cpp
// Team_AIGameMode.cpp: 수신된 서버 AI 이동 데이터를 몬스터 프록시 캐릭터에 연동 주입
void ATeam_AIGameMode::SetAIMove(const Protocol::S_AIMOVE& AIMovePkt)
{
	const uint64 ObjectId = AIMovePkt.info().object_id();
	ATeam_AICharacterBase** FindActor = Enemies.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	ATeam_AICharacter_Recv* enemy = Cast<ATeam_AICharacter_Recv>(*FindActor);
	if (enemy == nullptr)
		return;

	enemy->pos = AIMovePkt.info();
	FVector loc{ enemy->pos.x(), enemy->pos.y(), enemy->pos.z() };
	FRotator rot{ enemy->pos.pitch(), enemy->pos.yaw(), enemy->pos.roll() };

	enemy->RecvMoveTo(loc, rot, enemy->pos.speed());
}

// Team_AIGameMode.cpp: 서버의 안전 구역 정보를 실시간 자기장 맵 오브젝트의 수축 연산 단에 전달
void ATeam_AIGameMode::SetMagneticField(const Protocol::S_SET_MAGNETICFIELD& magPkt)
{
	FLinearColor rgba{ magPkt.r(), magPkt.g(),magPkt.b(),magPkt.a() };
	if (MagneticField)
		MagneticField->RecvUpdateSafetyFieldValue(rgba, magPkt.radius(), magPkt.time());
}
```

---

## 2. 서버 피드백 루프 기반의 BehaviorTree AI 및 EQS/Perception 전술 행동
* **파일명**: 
  - [Source/AI_MAP/Team_AI/Private/Team_AIController.cpp](https://github.com/ckdlscjs/BattleFront/blob/main/Source/AI_MAP/Team_AI/Private/Team_AIController.cpp) (OnAIMoveComplete, OnTargetDetect 및 OnSensed)
* **기능 개요**: 서버의 AI 행동 승인 패킷을 수신해 비헤이비어 트리(BT)와 블랙보드(BB)를 가동하고, 시야/피격 Perception 인지와 오버랩 구체 실시간 범위 검증을 수행하며, 행동 완료 시 완료 콜백(Move Done)을 서버로 전송하는 상호 피드백형 AI 프레임워크를 조율함.
* **코드 상세 분석**:
  - `OnPossess()`는 기동 시 몬스터의 스폰 위치와 기본 공격 대기 속성을 블랙보드 컴포넌트에 초기 세팅하며 비헤이비어 트리를 구동함.
  - `OnAIMoveComplete()`는 AI 몬스터가 NavMesh 상의 목적지에 완수 도달하면 기동하는 델리게이트 콜백으로, BTTaskNode의 성공 결과를 갱신하고 서버로 `SendTest("MoveEnd")` 패킷을 전송해 다음 순찰 및 행동 승인을 검증받는 루프를 완성함.
  - `OnSensed()`는 AIPerception의 Sight(시각) 및 Damage(피격) 감지 자극이 갱신될 시, 거리를 판별해 최적의 타깃을 색출하여 블랙보드의 `Target` 오브젝트 키에 실시간 인젝션해 주는 어그로 전이 설계를 다룸.
  - `OnTargetDetect()`는 몬스터가 전투 상태에 들어설 시 지정 타이머 주기마다 `OverlapMultiByProfile`을 가동해 플레이어가 탐색 반경 밖으로 나갔는지 기하학적으로 검증 및 감시함.

```cpp
// Team_AIController.cpp: AI Perception 컴포넌트의 시각/피격 감지 및 어그로 타깃 블랙보드 바인딩
void ATeam_AIController::OnSensed(const TArray<AActor*>& UpdatedActors)
{
	ATeam_AICharacterBase* AICharacter = Cast<ATeam_AICharacterBase>(GetPawn());
	if (!AICharacter)
		return;
	if (!Blackboard)
		return;
	for (const auto& actor : UpdatedActors)
	{
		if (!actor->ActorHasTag(AICharacter->GetTargetTag()))
			continue;
		FActorPerceptionBlueprintInfo PerceptionInfo;
		PerceptionComponent->GetActorsPerception(actor, PerceptionInfo);
		for (const auto& sense : PerceptionInfo.LastSensedStimuli)
		{
			if (UAISense_Sight::StaticClass() == UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), sense))
			{
				auto Target = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey));
				Target = !Target ? actor : (AICharacter->GetDistanceTo(actor) < AICharacter->GetDistanceTo(Target)) ? actor : Target;
				if (Cast<AGameCharacter>(Target)->IsDead())
				{
					Blackboard->SetValueAsObject(TargetKey, nullptr);
					return;
				}
				Blackboard->SetValueAsObject(TargetKey, Target);
			}

			if (UAISense_Damage::StaticClass() == UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), sense))
			{
				if (Cast<AGameCharacter>(actor)->IsDead())
				{
					Blackboard->SetValueAsObject(TargetKey, nullptr);
					return;
				}
				Blackboard->SetValueAsObject(TargetKey, actor);
			}
		}
	}
}
```

```cpp
// Team_AIController.cpp: AI 이동 완료 시 BTTask 성공 완료 처리 및 서버 완료 신호 전송 피드백
void ATeam_AIController::OnAIMoveComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	ATeam_AICharacterBase* AICharacter = Cast<ATeam_AICharacterBase>(GetPawn());
	if (!AICharacter)
		return;
        
	UBehaviorTreeComponent* BehaviorTree = Cast<UBehaviorTreeComponent>(GetBrainComponent());
	if (!BehaviorTree)
		return;
        
	BehaviorTree->OnTaskFinished(Cast<UBTTaskNode>(BehaviorTree->GetActiveNode()), EBTNodeResult::Succeeded);
	AICharacter->SendTest("MoveEnd");
    
	if(AICharacter->GetCurrentState() == ECharacterState::PATROL)
		AICharacter->IncrementPatrolIndex();
}
```

---

## 3. 월드 맵 설계 및 공간 환경/오브젝트 컬링 시스템
* **파일명**: 
  - [Source/AI_MAP/Team_AI/Private/Team_AIBuildingBase.cpp](https://github.com/ckdlscjs/BattleFront/blob/main/Source/AI_MAP/Team_AI/Private/Team_AIBuildingBase.cpp) (ActorBeginOverlap 지붕 컬링)
  - [Source/AI_MAP/Team_AI/Private/Team_AIBushCollision.cpp](https://github.com/ckdlscjs/BattleFront/blob/main/Source/AI_MAP/Team_AI/Private/Team_AIBushCollision.cpp) (BushBeginOverlap 수풀 투명 가시성 연동)
* **기능 개요**: 쿼터뷰 탑다운 카메라 뷰에 발생하는 지형지물 가시성 차단을 복구하기 위해, 충돌 오버랩 박스를 가동해 건물의 모든 지붕 StaticMesh 렌더링 가시성을 ON/OFF 컬링하고, 수풀 은닉 시 C++ 단에서 수풀 및 은닉 개체들의 가시성을 보간 셋업함.
* **코드 상세 분석**:
  - `Team_AIBuildingBase::PostInitializeComponents()`는 포스트 인스턴스 시점에 빌딩 액터 하위의 모든 `UStaticMeshComponent` (Roofs) 에셋 자원을 수집하여 컨테이너에 등록함.
  - `ActorBeginOverlap()` 및 `ActorEndOverlap()` 은 감지 오버랩 박스 충돌 틱을 활용해, 플레이어 진입 시 Roofs 메쉬 가시성(`SetVisibility`)을 꺼주어 천장이 사라지는 실내 투사 씬을 구현하고 이탈 시 즉시 실시간 복원함.
  - `ATeam_AIBushCollision` 은 플레이어 캐릭터의 수풀(Bush) 오버랩 상태(`PlayerInBush`)를 갱신하여, 수풀에 들어온 모든 아군/적대 개체들의 가시성 속성(`SetVisibility`)을 연동해 전술 은닉 상태를 동기화 시켜줌.

```cpp
// Team_AIBuildingBase.cpp: 오버랩 충돌 감지 기반 다중 건물 지붕 StaticMesh 일괄 투명/가시성 전환
void ATeam_AIBuildingBase::ActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (const auto& iter : Roofs)
	{
		iter->SetVisibility(false);
	}
}

void ATeam_AIBuildingBase::ActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (const auto& iter : Roofs)
	{
		iter->SetVisibility(true);
	}
}
```

```cpp
// Team_AIBushCollision.cpp: 캐릭터 수풀(Bush) 진입 여부에 의거한 개체 가시성 속성 동기화
void ATeam_AIBushCollision::BushBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto overlapActor = Cast<AGameCharacter>(OtherActor);
	Players.Add(overlapActor);
	if (Cast<ACharacterController>(overlapActor->GetController()) == Cast<ACharacterController>(GetWorld()->GetFirstPlayerController()))
		PlayerInBush = true;
	for (const auto& iter : Players)
	{
		Cast<AGameCharacter>(iter)->SetVisibility(PlayerInBush);
	}
}
```

---

## 4. 미니맵 시스템 및 시네마틱 카메라 제어
* **파일명**: 
  - [Source/AI_MAP/Team_AI/Private/Team_AIMapCamera.cpp](https://github.com/ckdlscjs/BattleFront/blob/main/Source/AI_MAP/Team_AIMapCamera.cpp) (BeginPlay 및 Tick 회전 카메라 제어)
* **기능 개요**: 레벨 진입 시 전장을 한눈에 웅장하게 조망하여 공간적 긴장감을 환기하기 위해, 상공 맵 카메라의 회전 반경 거리를 연산하고 삼각함수를 통한 원형 궤적 실시간 부감 뷰타겟 렌더를 처리함.
* **코드 상세 분석**:
  - `BeginPlay()`는 맵 카메라의 시작 위치와 전장 원점(ZeroVector) 간의 2D 평면 거리를 측정해 회전 반경(`Distance`)을 산정함.
  - `Tick()`은 매 프레임 타임 슬라이스(`rotTimeCur`) 누적을 통해 360도 비율(Ratio)을 기하 연산하고, 삼각함수 `cos`과 `sin`을 적용한 회전 좌표계 위치 성분을 맵 카메라 트랜스폼에 재세팅하며, `FindLookAtRotation()`을 기동해 카메라 뷰포트 방향이 항상 맵 중심을 사수하도록 갱신하여 쿼터뷰 시작 시네마틱을 연출함.

```cpp
// Team_AIMapCamera.cpp: 삼각함수 원형 궤적 360도 회전 연산 및 맵 중심 시선 포커싱
void ATeam_AIMapCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	rotTimeCur += DeltaTime;
	if(rotTimeCur > rotTime2Pi)
		rotTimeCur -= rotTime2Pi;

	float ratio = rotTimeCur / rotTime2Pi;
	FVector2D xyVec = FVector2D(cos(360 * ratio * PI / 180), sin(360 * ratio * PI / 180)) * Distance;
	SetActorLocation(FVector(xyVec.X, xyVec.Y, GetActorLocation().Z));
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector::ZeroVector));
}
```
