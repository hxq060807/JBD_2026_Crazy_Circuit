# 智能车加减速逻辑和Flash存储功能 - 产品需求文档

## Overview
- **Summary**: 实现智能车的加减速逻辑改进和第一圈里程记录的Flash存储功能，使智能车能够从最高速度平滑减速到Basic_Speed，并将第一圈记录的里程数据持久化存储。
- **Purpose**: 解决智能车在高速行驶时遇到转弯节点会打滑、甩尾的问题，并通过Flash存储避免重复记录里程。
- **Target Users**: 智能车竞赛参与者和开发者

## Goals
- 实现从设定的最高速度平滑减速到Basic_Speed的逻辑
- 将第一圈记录的里程数据存储到Flash中
- 系统重启后能够从Flash中读取之前记录的里程数据
- 优化智能车的转弯稳定性和速度控制

## Non-Goals (Out of Scope)
- 不修改现有的PID控制参数
- 不改变现有的赛道检测和转向逻辑
- 不增加新的硬件依赖

## Background & Context
- 当前智能车控制系统已经实现了基本的轨迹跟随和速度控制功能
- 现有加减速逻辑是基于当前速度和转弯节点的距离进行简单的线性减速
- 第一圈记录的里程数据仅存储在内存中，系统重启后会丢失

## Functional Requirements
- **FR-1**: 实现从最高速度平滑减速到Basic_Speed的逻辑
- **FR-2**: 将第一圈记录的里程数据存储到Flash中
- **FR-3**: 系统重启后能够从Flash中读取之前记录的里程数据
- **FR-4**: 优化提前减速逻辑，确保智能车在转弯时不会打滑、甩尾

## Non-Functional Requirements
- **NFR-1**: 加减速过程要平滑，避免急加速或急减速
- **NFR-2**: Flash存储操作要可靠，确保数据不会丢失
- **NFR-3**: 系统重启后能够正确识别和使用之前存储的里程数据
- **NFR-4**: 代码修改要最小化，避免影响现有功能

## Constraints
- **Technical**: 基于现有的智能车硬件平台和软件架构
- **Dependencies**: 依赖Flash操作相关的函数库

## Assumptions
- 智能车硬件平台支持Flash存储操作
- 现有Flash操作函数库能够正常工作
- 赛道布局在多次运行中保持不变

## Acceptance Criteria

### AC-1: 加减速逻辑改进
- **Given**: 智能车在赛道上行驶
- **When**: 接近转弯节点时
- **Then**: 智能车能够从最高速度平滑减速到Basic_Speed
- **Verification**: `human-judgment`
- **Notes**: 减速过程应该平滑，避免急刹车

### AC-2: 里程数据Flash存储
- **Given**: 智能车完成第一圈行驶
- **When**: 第一圈结束时
- **Then**: 系统将记录的里程数据存储到Flash中
- **Verification**: `programmatic`
- **Notes**: 存储操作应该成功完成，无错误

### AC-3: 里程数据Flash读取
- **Given**: 系统重启后
- **When**: 智能车开始行驶
- **Then**: 系统从Flash中读取之前存储的里程数据
- **Verification**: `programmatic`
- **Notes**: 读取操作应该成功完成，数据正确

### AC-4: 转弯稳定性
- **Given**: 智能车以高速接近转弯节点
- **When**: 执行转弯操作时
- **Then**: 智能车能够稳定通过转弯，不会打滑、甩尾
- **Verification**: `human-judgment`
- **Notes**: 转弯过程应该平稳，车身姿态保持良好

## Open Questions
- [ ] 最高速度的具体数值是多少？
- [ ] Flash存储的具体位置和格式是什么？
- [ ] 系统重启后如何检测是否存在之前存储的里程数据？