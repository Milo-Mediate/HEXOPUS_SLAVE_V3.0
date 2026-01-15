#ifndef __APP_H
#define __APP_H

#ifdef __cplusplus
extern "C" {
#endif

extern void app_init();  // ToDo: Definire un error code e restituirlo, il main lancerà l'error handler
extern void app_main();

#ifdef __cplusplus
}
#endif

#endif /* __APP_H */
