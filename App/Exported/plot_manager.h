/**
 * @file    plot_manager.h
 * @brief   Plot/stream utilities over UART or CAN
 * @ingroup app
 *
 * @details
 *  Provides a single entry point @ref plot_control() that:
 *   - On HEXOPAD builds: streams moving averages and thresholds via UART
 *     and CAN to plotting tools.
 *   - On MASTER builds: streams selected signals via UART for host plotting.
 */

#ifndef EXPORTED_PLOT_MANAGER_H_
#define EXPORTED_PLOT_MANAGER_H_


/**
 * @brief Periodic plot/stream handler.
 * @details
 *  - HEXOPAD: prints two traces and (optionally) sends MVG/TH pairs via CAN.
 *  - MASTER : prints up to 8 signals for visualization.
 */
void plot_control(void);


#endif /* SRC_PLOT_MANAGER_H_ */
