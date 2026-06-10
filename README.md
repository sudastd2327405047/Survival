# Survival (UE5.4)
基于 **Unreal Engine 5.4.4** 的第一人称射击项目，采用 **Blueprint + C++ 混合开发**。  
当前阶段以基地交互与玩法闭环搭建为主。
## 项目状态
- 工厂战斗地图：已有可玩蓝图逻辑（GameMode/UI/角色流程）
- 基地地图：已接入 C++ 交互框架（门/工作台/电视）
- 资源搜集地图：开发中
## 核心玩法循环
1. 前往资源地图搜集物资并带回基地出售  
2. 用金钱购买材料，在工作台制造装备与消耗品  
3. 在基地领取任务，进入工厂战斗并撤离
## 技术栈
- Unreal Engine 5.4.4
- C++
- Blueprint
- Visual Studio 2022
- Git + GitHub
## 主要 C++ 结构
- `Source/Survival/SurvivalCharacterBase.*`  
  玩家交互输入与目标选择（重叠 + 朝向）
- `Source/Survival/Interaction/InteractableActor.*`  
  交互基类
- `Source/Survival/Interaction/DoorInteractable.*`  
  门交互（选关 UI 入口）
- `Source/Survival/Interaction/WorkbenchInteractable.*`  
  工作台交互（制造 UI 入口）
- `Source/Survival/Interaction/TvInteractable.*`  
  电视交互（任务/商店 UI 入口）
## 本地开发流程（建议）
1. 修改 C++ 代码并保存  
2. 小改动：UE 编辑器内 `Compile`  
3. 大改动（新增 UCLASS / 改 Target/Build）：  
   - 关闭 UE
   - Generate Visual Studio project files
   - VS 编译 `SurvivalEditor | Win64 | Development`
4. 编译通过后进行 PIE 验证
## Git 策略（当前）
单人开发，仅维护 `main` 分支，每次代码更新直接提交到 `main`。
## 仓库收录策略
当前仓库只保留 **C++代码 + 必要配置**，不上传蓝图与美术资产。  
详情见 `.gitignore`。
保留：
- `.uproject`
- `Source/**`
- `Config/Default*.ini`
- `.gitignore`
忽略：
- `Content/`
- `Binaries/`
- `Intermediate/`
- `DerivedDataCache/`
- `Saved/`
- `.vs/`
- `.idea/`
## 仓库地址
[https://github.com/sudastd2327405047/Survival](https://github.com/sudastd2327405047/Survival)