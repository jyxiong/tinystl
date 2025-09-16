# Notes and Recommended Order for Implementing STL from Scratch

## Key Considerations

1. **Follow STL Design Principles**: Emphasize generic programming, separation of algorithms and data structures, and the iterator mechanism.
2. **Interface Compatibility**: Keep interfaces as close as possible to the standard STL for easier testing and replacement.
3. **Memory Management**: Implement your own allocator and pay attention to object construction and destruction.
4. **Exception Safety**: Ensure resources are released and states remain consistent in case of exceptions.
5. **Performance Optimization**: Avoid unnecessary copies and leverage move semantics (C++11 and above).
6. **Template Programming**: Use templates extensively, but be aware of the difficulty in debugging template errors.
7. **Iterator System**: Implement different types of iterators (input, output, forward, bidirectional, random access).
8. **Unit Testing**: Write comprehensive tests for each component as you implement it.

## Recommended Implementation Order

1. **Infrastructure**
   - Implement basic utilities such as type traits, `pair`, `swap`, `move`, and `forward`.
   - Implement a simple allocator.
2. **Iterators**
   - Implement iterator base classes and traits.
   - Implement common iterators (e.g., raw pointer, `reverse_iterator`).
3. **Containers**
   - Sequential containers: `array` → `vector` → `deque` → `list` → `forward_list`
   - Associative containers: `set` → `map` → `multiset` → `multimap`
   - Unordered containers (hash): `unordered_set`, `unordered_map`, etc.
   - Adapters: `stack`, `queue`, `priority_queue`
4. **Algorithms**
   - Implement common algorithms such as `sort`, `find`, `copy`, `swap`, `unique`, `accumulate`, etc.
5. **Other Components**
   - Implement utilities like `function`, `bind`, `mem_fn`, `reference_wrapper`.
   - Implement exception classes and error handling mechanisms.

**Tip:** After implementing each module, write corresponding unit tests and improve incrementally. Start with simple containers like `array` or `vector`, then move to more complex containers and algorithms.
