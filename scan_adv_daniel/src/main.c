/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
//#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/console/console.h>

#include <zephyr/bluetooth/mesh.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/controller.h>

static uint8_t mfg_data[] = { 0xff, 0xff, 0x01 };
static char   dongle1_addr[BT_ADDR_STR_LEN-1] = "E2:52:86:A9:3D:62";
// array of rssi values to store [0 = measured, 1 = received]
//int8_t bcon_1_rssi[2] = {}, bcon_2_rssi[2] = {}, bcon_3_rssi[2] = {};
static int8_t bcon_1_rssi;
//static int8_t incomingID;
static int8_t counter_rssi=0;



static const struct bt_data ad[] = {
	BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 3),
};



static bool cb_data(struct bt_data *data, void *user_data)
{
    bcon_1_rssi = (int8_t)data->data[2];
   // incomingID = (int8_t)data->data[0];

    return false;
}



static void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type,
		    struct net_buf_simple *buf)
{
	
	char addr_str[BT_ADDR_LE_STR_LEN];
	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));

	if ((!strncmp(addr_str, dongle1_addr, BT_ADDR_STR_LEN-1)) && !(counter_rssi==10)){ //BT_ADDR_STR_LEN is 18 for some reason..
		printk("Device found: %s (RSSI measured: %d)\n", addr_str, rssi);
		
		// Store the measurement dongle to DK RSSI
		//bcon_1_rssi[0]=rssi;
		// Request dual measurement, 1 means request
		mfg_data[0] = 1;
		// Store dual measurement
		bt_data_parse(buf,cb_data,(void *)addr);

		printk("Device: %s (RSSI received: %d)\n", addr_str, bcon_1_rssi);
		counter_rssi++;
	

	}
	
}


int main(void)
{
	//console_init();

	struct bt_le_scan_param scan_param = {
		.type       = BT_HCI_LE_SCAN_PASSIVE,
		.options    = BT_LE_SCAN_OPT_NONE,
		.interval   = 0x0010,
		.window     = 0x0010,
	};
	int err;
	char addr_s[BT_ADDR_LE_STR_LEN];
	bt_addr_le_t addr = {0};
	size_t count = 1;

	uint8_t x, y;

	printk("Starting the measurements...HERE WE GO!!!!!!!!!!!!!\n");



	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}

	printk("Bluetooth initialized\n");

	err = bt_le_scan_start(&scan_param, scan_cb);
	printk("scanning..");
	if (err) {
		printk("Starting scanning failed (err %d)\n", err);
		return 0;
	}

	do {
		k_sleep(K_MSEC(5000));

		/* Start advertising */
		err = bt_le_adv_start(BT_LE_ADV_NCONN_IDENTITY, ad, ARRAY_SIZE(ad),
				      NULL, 0);
		if (err) {
			printk("Advertising failed to start (err %d)\n", err);
			return 0;
		}
		bt_id_get(&addr, &count);
		bt_addr_le_to_str(&addr, addr_s, sizeof(addr_s));
		//printk("advertising as %s\n", addr_s);

		/*
		printk("\nEnter x coordinate: ");
		x = console_getchar();
		printk("You entered %c", x);

		printk("\nEnter y coordinate: ");
		y = console_getchar();
		printk("You entered %c", y);
*/

		k_sleep(K_MSEC(5000));

		err = bt_le_adv_stop();
		if (err) {
			printk("Advertising failed to stop (err %d)\n", err);
			return 0;
		}
	} while (1);
	return 0;
}
