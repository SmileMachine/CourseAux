# Offline Course Auxiliary System

## 简介

2022级数据结构课设：线下课程辅助系统。

**功能：**

1. 课程导航
   - 从当前位置到目标位置的最优路线规划
2. 课程信息管理和查询
   1. 资料、作业、考试、课程群的查询
   2. 资料、作业文件
      - 压缩、去重。
3. 课外信息管理和查询
4. 活动提醒
   1. 课内活动：在每天一定的时间点，提醒：
      1. 上课信息（第二天要上的课）
      2. 作业信息（待交作业、批改状态、作业分数）
      3. 上课需要带的资料
      4. 考试信息（如果有的话）。
   2. 课外活动
      1. 活动冲突检测
      2. 闹钟提醒

## 工作进度

- [ ] 用户接口
  - [x] 登录
  - [x] 主菜单
  - [ ] 针对不同用户的菜单
  - [ ] 其他功能的菜单...
- [ ] 多用户
  - [x] 用户信息的离线存储（csv）
  - [x] 用户信息读取（从csv）
  - [ ] 用户信息写入文件
  - [x] 登录认证
  - [ ] 信息在内存中的存储与查找（当前是数组和顺序查找）
- [ ] 摘要算法（用于匹配与去重）
  - [x] 字符串
  - [ ] 文件
- [ ] 文件压缩
- [ ] 程序的健壮性
  - [ ] 异常捕捉与处理
  - [ ] 读文件错误
  - [ ] 登录时找不到用户
- [ ] 日志
- [ ] 工程构建相关
  - [x] CMake初步
  - [x] 汉字编码支持（读取UTF-8源代码文件，编译为GBK可执行文件）。
  - [ ] 使用动态链接库
