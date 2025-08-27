# CxxSmallVec

> A lightweight, modern C++ alternative to Boost.Container’s `small_vector` — **faster, lighter, and more flexible.**

---

## ✨ 特点

* **轻量级**：单头文件实现，无需引入整个 Boost 库
* **现代 C++**：基于 C++17/20 特性 (`std::construct_at`, `std::destroy_at`, `if constexpr`)
* **栈优先**：小容量元素存储在栈内，避免频繁堆分配
* **堆回退**：超过容量时自动迁移到堆上，接口与 `std::vector` 接近
* **增强功能**：

  * `emplace_back`
  * `shrink_to_fit_stack()`（堆回退到栈）
  * 可选扩容策略（倍增、固定增量、自定义）
  * 可选 allocator 支持
* **调试友好**：可启用 debug 统计，追踪是否在栈/堆、迁移次数、利用率

---

## ⚖️ 与 Boost.Container `small_vector` 的比较

| 特性         | Boost `small_vector` | CxxSmallVec   |
| ---------- | -------------------- | ------------- |
| 依赖体积       | 需要 Boost.Container   | 单头文件，零依赖      |
| C++ 标准     | 兼容 C++03+            | 专注 C++17/20   |
| 接口         | STL 风格               | STL 风格 + 扩展   |
| 扩容策略       | 固定倍增                 | 可配置           |
| Debug 可观测性 | 无                    | 可选 debug hook |
 

---


## 📦 适用场景

* 解析器、编译器里的 token buffer
* 日志系统的小批量聚合
* 性能敏感、内存分配开销高的场景
* 嵌入式开发（可选禁用堆分配）

 
 
 
