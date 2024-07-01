# BattleFront
UE5.1 TeamProject

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/c95220f4-b9c7-4321-81fb-a268319fc99d)

**역할 및 팀 구성**
- Perforce, AI, Map, Gameflow
- 개발 인원 : 3인

**영상**

https://youtu.be/uDiQvUdp6hE?si=ac6YNenHf0u3xg1y(TheBattleFront)

**게임**

http://naver.me/xf5U9K8l

## 🖋️담당 업무

- **Perforce**

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/2b433f0f-ec87-4f03-bbbd-e638efa8e825)

-Perforce Server를 구성, Mainlane(TeamProject)에서 Development브랜치를 구성, 형상관리에 사용

- **AI**

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/d40d33f9-8fcb-4ce5-9e03-3297a5d48b83)
-BehaviorTree를 기반으로 AIController의 행동을 제어

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/47c14707-249a-432c-9afe-d524b83b5f22)
![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/e8437590-66d8-4dc7-b810-7877343c0e1b)
-Perception, CollisionOverlap을 통해 Player를 감지

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/c1f0edb1-ddc4-4a46-9602-88bb28ece27b)
-EnvironmentQuerySystem사용으로 Wall(StaticMesh)와 같은 환경을 통해 Player의 공격을 회피

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/3a357c40-5e94-4808-a631-26d2fb5dcaa9)
-SpawnPoint Actor를 정의해 Enemy,Player를 생성한다.

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/95ac57f8-70e5-4609-bb67-83ef3cbf6852)
-Enemy와 Player는 구현한 IOCP서버를 통해 패킷을 주고받으며 다른 클라이언트와 통신
-여러 종류의 Enemy와 이에 대한 패킷을 받는 RecvEnemy를 구성해 서버 통신에 사용

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/33fd816f-d046-4bea-95b0-2eb57b3f70be)
-ABP(AnimInstance)를 통해 애니메이션을 제어하며 AnimMontage를 이용해 특정 상태를 지정.

- **Map**

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/8be36fde-659c-4fd3-aedc-bf10f3ba47af)
-Landscape, BlendMaterial을 이용해 맵기반을 제작, Collision과 Foliage를 이용해 맵 전반의 디테일작업.

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/d69c2adc-ae7b-4440-95ba-3e35f3d7c8eb)
-Building Actor를 정의해 가지고 있는 Roof에 해당하는 StaticMesh를 Collision판정을 이용해 캐릭터 접근시 ON,OFF

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/b01479d9-24db-45db-8b44-ef0ae7b7d786)
-CustomDepth, PostProcessing을 이용해 스텐실버퍼 를 이용, MaterialShader를 구성해 캐릭터의 위치를 기반으로 Depth버퍼 사용과 
BushMaterial의 MaterialParameterCollection을 통해 캐릭터의 위치를 계산해 Bush기능 구현

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/da8d5220-caa1-49e2-9c3a-4ddb4c960af3)
-포스트프로세싱을 이용해 Outline, FogOfWar 를 구현

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/9e6f3a2d-fc6e-4be1-b09b-21aa741853a2)
-Dissolve Material을 구성, Item획득시 Dissolve

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/6a8ebb12-94fa-49f6-bb99-3ed495aca59d)
-WorldMap, Minimap을 SceneCaptureComponent를 이용해 구성, 렌더타겟을이용해 렌더

![image](https://github.com/ckdlscjs/BattleFront/assets/41976800/719e27b6-d1bc-406c-ad3a-8f657f7d792d)
-자기장(MagneticField)기능 구현
