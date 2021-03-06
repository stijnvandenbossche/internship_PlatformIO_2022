

#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

#ifndef NULL
#ifndef __cplusplus
#define NULL (void*)0
#else
#define NULL 0
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

void unittest_uart_begin();
void unittest_uart_putchar(char c);
void unittest_uart_flush();
void unittest_uart_end();


#define UNITY_OUTPUT_START()    unittest_uart_begin()
#define UNITY_OUTPUT_CHAR(c)    unittest_uart_putchar(c)
#define UNITY_OUTPUT_FLUSH()    unittest_uart_flush()
#define UNITY_OUTPUT_COMPLETE() unittest_uart_end()


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* UNITY_CONFIG_H */