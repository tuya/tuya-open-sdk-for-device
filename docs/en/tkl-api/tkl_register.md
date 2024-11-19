# REGISTER DRIVER

## Brief Description

The register driver is Tuya's unified register operation interface, used for reading from and writing to internal chip registers.

## API Description

### tkl_reg_read

```c
uint32_t tkl_reg_read(uint32_t addr);
```

- Function Description:
  - Reads register data.
- Parameters:
  - `addr`: Register address.
- Return Value:
  - Register data.

### tkl_reg_bit_read

```c
uint32_t tkl_reg_bit_read(uint32_t addr, TUYA_ADDR_BITS_DEF_E start_bit, TUYA_ADDR_BITS_DEF_E end_bit);
```

- Function Description:
  - Reads the data of the corresponding bit in the register.
- Parameters:
  - `addr`: Register address.
  - `start_bit`: Starting bit of the register, refer to the TUYA_ADDR_BITS_DEF_E definition.
  - `end_bit`: Ending bit of the register, refer to the TUYA_ADDR_BITS_DEF_E definition.
- Return Value:
  - Data of the corresponding bit in the register.

### tkl_reg_write

```c
OPERATE_RET tkl_reg_write(uint32_t addr, uint32_t data);
```

- Function Description:
  - Writes data to the register.
- Parameters:
  - `addr`: Address of the register to write to.
  - `data`: Data to be written.
- Return Value:
  - OPRT_OK for success, other values please refer to the file `tuya_error_code.h`.

### tkl_reg_bit_write

```c
OPERATE_RET tkl_reg_bit_write(uint32_t addr, TUYA_ADDR_BITS_DEF_E start_bit, TUYA_ADDR_BITS_DEF_E end_bit, uint32_t data);
```

- Function Description:
  - Writes data to the register.
- Parameters:
  - `addr`: Address of the register to write to.
  - `start_bit`: Starting bit of the register, refer to the TUYA_ADDR_BITS_DEF_E definition.
  - `end_bit`: Ending bit of the register, refer to the TUYA_ADDR_BITS_DEF_E definition.
  - `data`: Data to be written.
- Return Value:
  - OPRT_OK for success, other values please refer to the file `tuya_error_code.h`.
