# 项目上下文

## 项目简介
本项目基于 UE5.4.4，类型为第一人称射击（FPS），采用“蓝图 + C++”混合开发。

当前地图规划：
1. 基地（Basement）：交互、制造、任务与商店中枢  
2. 战斗地图（工厂）：主要战斗与撤离流程  
3. 资源搜集地图：资源容器搜集与回收循环（开发中）

核心玩法循环：
1. 玩家前往资源地图搜集物资并带回基地售卖获得金钱  
2. 使用金钱购买制造材料，在基地工作台制作武器/弹药/防具/药剂  
3. 在基地领取任务，进入工厂执行战斗并撤离

## 当前开发进度
1. 工厂地图：GameMode、UI、角色逻辑主要由蓝图实现，已具备完整可玩流程  
2. 基地地图：交互框架已迁移到 C++ 基类，蓝图负责具体 UI 打开逻辑  
   - 玩家蓝图已 Reparent 到 `ASurvivalCharacterBase`
   - 门蓝图父类：`ADoorInteractable`
   - 工作台蓝图父类：`AWorkbenchInteractable`
   - 电视蓝图父类：`ATvInteractable`
3. 资源搜集地图：暂未开始
4. 仓库UI（仿三角洲，目前暂时设置一个物品为一格，可拖拽交互、交换位置，不可重叠）已经用c++实现，ui暂时没做，蓝图没做。

---

## 技术栈

### 引擎与语言
- Unreal Engine 5.4.4
- C++（Gameplay Framework / UCLASS）
- Blueprint（UI 与玩法快速迭代）

### 开发工具
- Cursor（主编辑器）
- Visual Studio 2022（构建/调试）
- Git + GitHub（版本管理）

---

## 开发规范

### 代码规范
1. 交互系统约束：
   - C++ 负责交互检测、目标筛选、交互事件分发
   - 蓝图负责具体界面打开、界面逻辑与美术联动
2. 角色交互输入：
   - 统一使用 `Interact` Action Mapping（默认 E）
3. 可扩展原则：
   - 新交互对象优先继承 `AInteractableActor`
   - 对象行为通过 BlueprintImplementableEvent 扩展，不在基类硬编码 UI 资产
4. 变更原则：
   - 优先小步修改，保持可编译、可运行、可回滚
5. 注释
   -所有代码打上中文注释，尤其是需要暴露给蓝图的函数

### 编译规范（每次改代码后）
1. 小改动（函数实现/少量逻辑）：
   - 先在 UE Editor 点 `Compile`
   - 再进行 PIE 验证
2. 大改动（新增UCLASS、改Build/Target、模块配置）：
   - 关闭 UE
   - 重新生成项目文件（Generate Visual Studio project files）
   - VS 编译 `SurvivalEditor | Win64 | Development`
   - 成功后再打开 `.uproject`

---

## Git 规范（单人 main 分支模式）

### 分支策略
- 单人开发，仅使用 `main` 分支
- 每次可运行的代码更新都直接提交到 `main`
- 不创建长期开发分支

### 提交策略
- 每次提交保持“单一目的”（一个功能点 / 一个修复点）
- 提交前必须满足：
  1. 代码可编译
  2. 项目可打开
  3. 关键路径可在 PIE 验证
- 推荐提交信息格式：
  - `feat: basement add interactable base classes`
  - `fix: resolve module rebuild mismatch on editor startup`
  - `chore: update target build settings for local toolchain`

### 远程仓库
- GitHub 仓库：`https://github.com/sudastd2327405047/Survival`
- 当前策略：仅上传 C++ 代码与必要配置，不上传蓝图和美术资产

---

## 仓库内容策略（与 .gitignore 对齐）

当前仓库目标：
- 保留：
  - `.uproject`
  - `Source/**`（C++ 代码）
  - `Config/Default*.ini`
  - `.gitignore`
- 忽略：
  - `Content/`（蓝图与资产）
  - `Binaries/`
  - `Intermediate/`
  - `DerivedDataCache/`
  - `Saved/`
  - `.vs/`
  - `.idea/`

说明：
- 该策略适合“代码仓库化、资产本地化”开发模式
- 若未来需要团队协作与可直接运行工程，应重新评估是否纳入部分蓝图与关卡资源

---

## 项目结构（当前重点）

- `Source/Survival/`
  - `SurvivalCharacterBase.*`：玩家交互输入与目标筛选
  - `Interaction/InteractableActor.*`：交互基类
  - `Interaction/DoorInteractable.*`：门交互类
  - `Interaction/WorkbenchInteractable.*`：工作台交互类
  - `Interaction/TvInteractable.*`：电视交互类
- `Config/`
  - `Default*.ini`：项目基础配置
- `Content/`
  - 当前不纳入 Git（本地资产目录）

---

## 待办方向（建议）
1. 基地交互提示（按键提示、可交互高亮）
2. 工作台制造数据表与配方系统
3. 任务系统数据化（任务状态、奖励、刷新）
4. 资源地图与基地经济闭环联调
5. 工厂地图战斗平衡与撤离流程优化