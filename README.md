# BattleFront
UE5.1 TeamProject

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/5998b30d-44b9-4130-bab1-7334f1989267/Untitled.png)

**역할 및 팀 구성**

- Perforce, AI, Map, Gameflow
- 개발 인원 : 3인, 팀장

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/a8e77737-8a87-4df2-a78c-c3ae9b9787fa/Untitled.png)

~~📎~~**영상**

https://youtu.be/uDiQvUdp6hE?si=ac6YNenHf0u3xg1y(TheBattleFront)

📎**게임**

http://naver.me/xf5U9K8l

**~~**📎~~**Github**

https://github.com/ckdlscjs/BattleFront

# 📝상세 내용

---

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/16eab047-61d4-47c3-a63f-fe639303e322/Untitled.png)

<aside>
📢 TheBattleFront 는 게임 “VampireSuvivors”와 “EternalReturn”에서 아이디어를 착안해
제작한 게임입니다.
PvP게임으로 맵에서 스폰 되는 AI와 전투하며 레벨을 올리고 여러 Player들과
최후의 1인 이 될 때 까지 생존 하는 게임입니다.

</aside>

## 🖋️담당 업무

- **Perforce**

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/7b5033a9-1d5c-4843-a145-492a272be785/Untitled.png)

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/c795e2ec-7719-49bd-8a6a-8964699f58a2/Untitled.png)

-Perforce Server를 구성, Mainlane(TeamProject)에서 Development브랜치를 구성, 형상관리에 사용

- **AI**

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/9348ab2e-396b-4577-a621-a4725a6796e5/Untitled.png)

-BehaviorTree를 기반으로 AIController의 행동을 제어한다.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/42eb6f4e-9b49-482d-a54a-673aecb0253f/Untitled.png)

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/916c0309-5409-478e-8234-42c2aeebdad0/Untitled.png)

-Perception, CollisionOverlap을 통해 Player를 감지

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/0ebafa83-459d-44b1-b125-319e5122f947/Untitled.png)

-EnvironmentQuerySystem사용으로 Wall(StaticMesh)와 같은 환경을 통해 Player의 공격을 회피

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/e72db4b5-eeff-4d6a-84d5-e7f2f86971e7/Untitled.png)

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/2d7458f2-15fc-412f-94a8-be8a2a0d7b31/ab5a4903-a7e2-463a-876f-a215023c8004.png)

-SpawnPoint Actor를 정의해 Enemy,Player를 생성한다.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/43a6a91f-0788-4386-b56e-5f4c5d5f225a/Untitled.png)

-Enemy와 Player는 구현한 IOCP서버를 통해 패킷을 주고받으며 다른 클라이언트와 통신

-여러 종류의 Enemy와 이에 대한 패킷을 받는 RecvEnemy를 구성해 서버 통신에 사용

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/4193fb25-366d-439b-a897-da7a1455dfaa/Untitled.png)

-ABP(AnimInstance)를 통해 애니메이션을 제어하며 AnimMontage를 이용해 특정 상태를 지정.

- **Map**

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/f08b5281-c4f9-439f-9e77-242a7ebbb89f/Untitled.png)

-Landscape, BlendMaterial을 이용해 맵기반을 제작, Collision과 Foliage를 이용해 맵 전반의 디테일작업.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/238b3545-d69b-4a73-abf8-cd8c2b951d4d/Untitled.png)

-Building Actor를 정의해 가지고 있는 Roof에 해당하는 StaticMesh를 Collision판정을 이용해

캐릭터 접근시 ON,OFF

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/feaa79e4-60e1-47e0-bbf3-76d54d5841d9/Untitled.png)

-CustomDepth, PostProcessing을 이용해 스텐실버퍼 를 이용, MaterialShader를 구성해
캐릭터의 위치를 기반으로 Depth버퍼 사용과 BushMaterial의 MaterialParameterCollection을

통해 캐릭터의 위치를 계산해 Bush기능 구현

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/7fd90580-acca-4f89-afd4-8f5d8b476acc/Untitled.png)

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/1a0db9b8-d66e-4aaa-b5b0-4ed320e754f6/Untitled.png)

-포스트프로세싱을 이용해 Outline, FogOfWar 를 구현

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/8db5e9f2-d611-4b6e-84a9-472ac17ac6c3/Untitled.png)

-Dissolve Material을 구성, Item획득시 Dissolve

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/f1209b41-0ca3-418b-8318-8af3fa622cb5/Untitled.png)

-WorldMap, Minimap을 SceneCaptureComponent를 이용해 구성, 렌더타겟을이용해 렌더

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/2c037e11-5d69-451f-8557-93550e6a2b52/1270cd6c-eb2f-4e18-9eb5-52b3fc4be338/Untitled.png)

-자기장(MagneticField)기능 구현
