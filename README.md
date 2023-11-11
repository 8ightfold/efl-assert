# efl-assert

This library's purpose is simple: it extends ``assert``.
It allows you to swap this:

```cpp
assert(cond && "Message");
```

with this:

```cpp
EflAssert(cond, "Message");
```

It does this by locating ``assert.h``, extracting the debug definition,
and then replacing the default stringification with a custom message.

To get started, just do:

```cmake
add_subdirectory(efl-assert)
...
target_link_libraries(your-exe efl::assert)
```

## Dependencies

``efl::assert`` has to parse ``assert.h``, so a scripting language is required.
Because of this, you need ``Perl`` installed and configured for CMake usage.

If you set ``EFL_ASSERT_FALLBACK`` to ``ON``, the fallback path
will immediately be taken (though this does defeat the purpose of the library).

## C++ Usage

``<efl/Assert.hpp>`` defines a few things:

```cpp
#define EflAssert(check, message) ...
#define EflDynAssert(check, to-format...) \
    EflAssert(check, EflDynAssertFormatter(to-format))
#define EflDynAssertFormatter(to-format...) ...

#define EflDbgAssert(check, message)
#define EflDbgDynAssert(check, to-format...)
```

``EflAssert`` is like normal ``assert``, except a custom message
is printed as the error.

``EflDynAssert`` will pass its arguments to ``EflDynAssertFormatter``,
which get converted into a local string. You can provide your own
definition for the formatting, just make sure it evaluates to a
``const char*``, or something that can be implicitly converted to it.

The ``Dbg`` variants of both functions work the same in debug mode,
but if ``NDEBUG`` is defined, they become noops.
