# Project Architecture

`DMGL` contains a series of subsystems, each representing one component of the physical hardware in the Gameboy. The subsystems communicate with each other through, and are driven by, a central bus and service. The bus and service interface to the outside world for configuration and IO. The following diagram gives a high-level overview of how all the subsystems fit together:

![Architecture](https://github.com/majestic53/dmgl/blob/master/docs/arch.png "Architecture")

## File layout

Subsystem source files can be found under the following directories:

|Directory                                                                              |Description                          |
|:--------------------------------------------------------------------------------------|:------------------------------------|
|[`src/common`](https://github.com/majestic53/dmgl/blob/master/src/common)              |Common source files                  |
|[`src/service`](https://github.com/majestic53/dmgl/blob/master/src/service)            |Service source files (SDL by default)|
|[`src/system`](https://github.com/majestic53/dmgl/blob/master/src/system)              |Subsystem source files               |
|[`src/system/mapper`](https://github.com/majestic53/dmgl/blob/master/src/system/mapper)|Mapper-specific source files         |

## Testing

Each subsystem has unittests, found under [`test`](https://github.com/majestic53/dmgl/blob/master/test)

## Coding style

The source code generally conforms to the following coding standard:

### Enumerations

```c
/*!
 * @enum dmgl_xxx_e
 * @brief ...
 */
typedef enum {
    DMGL_AAA = 0,   /*!< Doxygen comment */
    DMGL_BBB,       /*!< Doxygen comment */
    ...
} dmgl_xxx_e;
```

### Structures

```c
/*!
 * @struct dmgl_xxx_t
 * @brief ...
 */
typedef struct {
    dmgl_xxx_e val_0;   /*!< Doxygen comment */
    int val_1;          /*!< Doxygen comment */
    ...
} dmgl_xxx_t;
```

### Functions

```c
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

...

/*!
 * @brief ...
 * @param[in,out] arg_0 ...
 * @param[in] arg_1 ...
 * @return ...
 */
int dmgl_xxx_yyy(dmgl_xxx_t *arg_0, dmgl_xxx_e arg_1, ...)
{
    int result = DMGL_SUCCESS;

    if(arg_0 == NULL) {
        result = DMGL_FAILURE;
        goto exit;
    }

    ...

exit:
    return result;
}

...

#ifdef __cplusplus
}
#endif /* __cplusplus */
```
