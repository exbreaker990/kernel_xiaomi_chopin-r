
/*
 * Copyright (C) 2016 MediaTek Inc.
 * Copyright (C) 2021 XiaoMi, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef _MTK_EEM_INTERNAL_AP_H_
#define _MTK_EEM_INTERNAL_AP_H_

struct eem_det;
struct eem_ctrl;

struct eem_det_ops {
	/* interface to EEM */
	void (*enable)(struct eem_det *det, int reason);
	void (*disable)(struct eem_det *det, int reason);
	void (*disable_locked)(struct eem_det *det, int reason);
	void (*switch_bank)(struct eem_det *det, enum eem_phase phase);

	int (*init01)(struct eem_det *det);
	int (*init02)(struct eem_det *det);
	int (*mon_mode)(struct eem_det *det);

	int (*get_status)(struct eem_det *det);
	void (*dump_status)(struct eem_det *det);

	void (*set_phase)(struct eem_det *det, enum eem_phase phase);

	/* interface to thermal */
	int (*get_temp)(struct eem_det *det);

	/* interface to DVFS */
	int (*get_volt)(struct eem_det *det);
	int (*set_volt)(struct eem_det *det);
	void (*restore_default_volt)(struct eem_det *det);
	void (*get_freq_table)(struct eem_det *det);
	void (*get_orig_volt_table)(struct eem_det *det);

	/* interface to PMIC */
	int (*volt_2_pmic)(struct eem_det *det, int volt);
	int (*volt_2_eem)(struct eem_det *det, int volt);
	int (*pmic_2_volt)(struct eem_det *det, int pmic_val);
	int (*eem_2_pmic)(struct eem_det *det, int eev_val);
};

struct eem_aee_volt_det {
	unsigned int cur_entry;
	char volt_entry[AEE_ENTRY_NUM][AEE_PER_ENTRY_LEN];
	char dumpbuf[PTP_MEM_SIZE];
};

struct eem_aee_volt_history {
	struct eem_aee_volt_det aee_v_det[NR_EEM_DET];
};

struct eem_phase_efuse {

	unsigned int MDES:8;
	unsigned int BDES:8;
	unsigned int DCMDET:8;
	unsigned int DCBDET:8;
	unsigned int MTDES:8;
	unsigned int str_pt:8;
	unsigned int end_pt:8;
	unsigned int is_str_fnd:1;
};

struct eem_det {
	const char *name;
	struct eem_det_ops *ops;
	int status;			/* TODO: enable/disable */
	int features;		/* enum eem_features */
	enum eem_ctrl_id ctrl_id;

	/* devinfo */
	unsigned int EEMINITEN;
	unsigned int EEMMONEN;

	unsigned int SPEC;
	struct eem_phase_efuse phase_ef[NR_EEM_PHASE];

	/* constant */
	unsigned int DETWINDOW;
	unsigned int VMAX;
	unsigned int VMIN;
	unsigned int DTHI;
	unsigned int DTLO;
	/* unsigned int VBOOT; */
	unsigned int DETMAX;
	unsigned int AGECONFIG;
	unsigned int AGEM;
	unsigned int DVTFIXED;
	unsigned int VCO;
	unsigned int DCCONFIG;

	/* Generated by EEM init01. Used in EEM init02 */
	unsigned int DCVOFFSETIN;
	unsigned int AGEVOFFSETIN;
	unsigned int EEMCTL0;

	/* for PMIC */
	unsigned int eem_v_base;
	unsigned int eem_step;
	unsigned int pmic_base;
	unsigned int pmic_step;

	/* for debug */
	/* unsigned int dcvalues[NR_EEM_PHASE]; */

	unsigned int freqpct30[NR_EEM_PHASE];
	unsigned int eem_26c[NR_EEM_PHASE];
	unsigned int vop30[NR_EEM_PHASE];
	unsigned int eem_eemEn[NR_EEM_PHASE];
	int temp; /* temperature */
	/* unsigned int real_vboot; */

#if DUMP_DATA_TO_DE
	unsigned int reg_dump_data[ARRAY_SIZE(reg_dump_addr_off)][NR_EEM_PHASE];
#endif
	/* slope */
	unsigned int MTS;
	unsigned int BTS;
	unsigned int t250;
	/* dvfs */
#if 0
	unsigned int init2_vop30;
	unsigned int init2_vop74;
	unsigned int mon_vop30;
	unsigned int mon_vop74;
#endif
	/* could be got @ the same time with freq_tbl[] */
	unsigned int num_freq_tbl;
	/* maximum frequency used to calculate percentage */
	unsigned int max_freq_khz;
	unsigned int turn_freq_khz[NR_EEM_PHASE];
	unsigned char freq_tbl[NR_FREQ]; /* percentage to maximum freq */
	/* orig volt table for restoreing to dvfs*/
	unsigned int volt_tbl_orig[NR_FREQ];
	unsigned int volt_tbl[NR_FREQ]; /* pmic value */
	/* unsigned int volt_tbl_init2[NR_FREQ]; */ /* pmic value */
	unsigned int volt_tbl_pmic[NR_FREQ]; /* pmic value */
	int volt_aging[NR_FREQ]; /* aging margin value */
#if 0
	struct mutex *loo_mutex;
	unsigned char loo_role;
	unsigned char loo_couple;
	unsigned char turn_pt;

	/* Picachu */
	unsigned int pi_efuse;

	unsigned int pi_dvtfixed : 16;
	unsigned int pi_loo_enabled: 1;
	unsigned int pi_reserved: 15;
#endif
	unsigned char init2_phase;
	unsigned char total2_phase;

	int volt_offset:8;
	int volt_clamp:8;

	unsigned int disabled:8; /* Disabled by error or sysfs */
	unsigned int low_temp_off:8;
	unsigned int high_temp_off:8;
	unsigned int high_temp85_off:8;
	unsigned int isTempInv:2;
	/* unsigned int volt_policy:1; */
	/* only when init2, eem need to set volt to upower */
	unsigned int set_volt_to_upower:1;
};

struct eem_devinfo {
	/* M_HW_RES0 0x11c1_0580 */
	unsigned int FT_PGM:8;
	unsigned int FT_BIN:4;
	unsigned int RSV0_1:20;

	/* M_HW_RES1 */
	unsigned int CPU_L_HI_MTDES:8;
	unsigned int CPU_L_HI_INITEN:1;
	unsigned int CPU_L_HI_MONEN:1;
	unsigned int CPU_L_HI_DVFS_LOW:3;
	unsigned int CPU_L_HI_SPEC:3;
	unsigned int CPU_L_HI_BDES:8;
	unsigned int CPU_L_HI_MDES:8;

	/* M_HW_RES2 */
	unsigned int CPU_L_MID_MTDES:8;
	unsigned int CPU_L_MID_INITEN:1;
	unsigned int CPU_L_MID_MONEN:1;
	unsigned int CPU_L_MID_DVFS_LOW:3;
	unsigned int CPU_L_MID_SPEC:3;
	unsigned int CPU_L_MID_BDES:8;
	unsigned int CPU_L_MID_MDES:8;

	/* M_HW_RES3 */
	unsigned int CPU_BL_HI_MTDES:8;
	unsigned int CPU_BL_HI_INITEN:1;
	unsigned int CPU_BL_HI_MONEN:1;
	unsigned int CPU_BL_HI_DVFS_LOW:3;
	unsigned int CPU_BL_HI_SPEC:3;
	unsigned int CPU_BL_HI_BDES:8;
	unsigned int CPU_BL_HI_MDES:8;

	/* M_HW_RES4 */
	unsigned int CPU_BL_MID_MTDES:8;
	unsigned int CPU_BL_MID_INITEN:1;
	unsigned int CPU_BL_MID_MONEN:1;
	unsigned int CPU_BL_MID_DVFS_LOW:3;
	unsigned int CPU_BL_MID_SPEC:3;
	unsigned int CPU_BL_MID_BDES:8;
	unsigned int CPU_BL_MID_MDES:8;

	/* M_HW_RES5 */
	unsigned int CPU_BL_LO_MTDES:8;
	unsigned int CPU_BL_LO_INITEN:1;
	unsigned int CPU_BL_LO_MONEN:1;
	unsigned int CPU_BL_LO_DVFS_LOW:3;
	unsigned int CPU_BL_LO_SPEC:3;
	unsigned int CPU_BL_LO_BDES:8;
	unsigned int CPU_BL_LO_MDES:8;

	/* M_HW_RES6 */
	unsigned int CPU_BL_ULV_MTDES:8;
	unsigned int CPU_BL_ULV_INITEN:1;
	unsigned int CPU_BL_ULV_MONEN:1;
	unsigned int CPU_BL_ULV_DVFS_LOW:3;
	unsigned int CPU_BL_ULV_SPEC:3;
	unsigned int CPU_BL_ULV_BDES:8;
	unsigned int CPU_BL_ULV_MDES:8;

	/* M_HW_RES7 */
	unsigned int CPU_B_HI_MTDES:8;
	unsigned int CPU_B_HI_INITEN:1;
	unsigned int CPU_B_HI_MONEN:1;
	unsigned int CPU_B_HI_DVFS_LOW:3;
	unsigned int CPU_B_HI_SPEC:3;
	unsigned int CPU_B_HI_BDES:8;
	unsigned int CPU_B_HI_MDES:8;

	/* M_HW_RES8 */
	unsigned int CPU_B_MID_MTDES:8;
	unsigned int CPU_B_MID_INITEN:1;
	unsigned int CPU_B_MID_MONEN:1;
	unsigned int CPU_B_MID_DVFS_LOW:3;
	unsigned int CPU_B_MID_SPEC:3;
	unsigned int CPU_B_MID_BDES:8;
	unsigned int CPU_B_MID_MDES:8;

	/* M_HW_RES9 */
	unsigned int CPU_B_LO_MTDES:8;
	unsigned int CPU_B_LO_INITEN:1;
	unsigned int CPU_B_LO_MONEN:1;
	unsigned int CPU_B_LO_DVFS_LOW:3;
	unsigned int CPU_B_LO_SPEC:3;
	unsigned int CPU_B_LO_BDES:8;
	unsigned int CPU_B_LO_MDES:8;

	/* M_HW_RES10 */
	unsigned int CCI_HI_MTDES:8;
	unsigned int CCI_HI_INITEN:1;
	unsigned int CCI_HI_MONEN:1;
	unsigned int CCI_HI_DVFS_LOW:3;
	unsigned int CCI_HI_SPEC:3;
	unsigned int CCI_HI_BDES:8;
	unsigned int CCI_HI_MDES:8;


	/* M_HW_RES11 */
	unsigned int CCI_MID_MTDES:8;
	unsigned int CCI_MID_INITEN:1;
	unsigned int CCI_MID_MONEN:1;
	unsigned int CCI_MID_DVFS_LOW:3;
	unsigned int CCI_MID_SPEC:3;
	unsigned int CCI_MID_BDES:8;
	unsigned int CCI_MID_MDES:8;

	/* M_HW_RES12 */
	unsigned int CCI_LO_MTDES:8;
	unsigned int CCI_LO_INITEN:1;
	unsigned int CCI_LO_MONEN:1;
	unsigned int CCI_LO_DVFS_LOW:3;
	unsigned int CCI_LO_SPEC:3;
	unsigned int CCI_LO_BDES:8;
	unsigned int CCI_LO_MDES:8;

	/* M_HW_RES13 */
	unsigned int GPU_DCBDET:8;
	unsigned int GPU_DCMDET:8;
	unsigned int CPU_L_HI_DCBDET:8;
	unsigned int CPU_L_HI_DCMDET:8;


#if 0
	/* M_HW_RES13 */
	unsigned int GPU_MID_DCBDET:8;
	unsigned int GPU_MID_DCMDET:8;
	unsigned int GPU_HI_DCBDET:8;
	unsigned int GPU_HI_DCMDET:8;

	/* M_HW_RES14 (org:8) */
	unsigned int GPU_MTDES:8;
	unsigned int GPU_INITEN:1;
	unsigned int GPU_MONEN:1;
	unsigned int GPU_DVFS_LOW:3;
	unsigned int GPU_SPEC:3;
	unsigned int GPU_BDES:8;
	unsigned int GPU_MDES:8;

	/* M_HW_RES15 (org:9) */
	unsigned int GPU_HI_MTDES:8;
	unsigned int GPU_HI_INITEN:1;
	unsigned int GPU_HI_MONEN:1;
	unsigned int GPU_HI_DVFS_LOW:3;
	unsigned int GPU_HI_SPEC:3;
	unsigned int GPU_HI_BDES:8;
	unsigned int GPU_HI_MDES:8;

	/* M_HW_RES16 (org:10) */
	unsigned int GPU_LO_MTDES:8;
	unsigned int GPU_LO_INITEN:1;
	unsigned int GPU_LO_MONEN:1;
	unsigned int GPU_LO_DVFS_LOW:3;
	unsigned int GPU_LO_SPEC:3;
	unsigned int GPU_LO_BDES:8;
	unsigned int GPU_LO_MDES:8;
#endif
};

/*********************************************
 *extern variables defined at mtk_eem.c
 *********************************************
 */
extern unsigned int freq[NR_FREQ];

extern struct mutex record_mutex;
#if ENABLE_LOO_B
extern struct mutex bcpu_mutex;
#endif
#if ENABLE_LOO_G
/* extern struct mutex lcpu_mutex; */
extern struct mutex gpu_mutex;
#endif

extern void mt_record_lock(unsigned long *flags);
extern void mt_record_unlock(unsigned long *flags);

/* table used to apply to dvfs at final */
extern unsigned int record_tbl_locked[NR_FREQ];

/**************************************************
 *extern variables and operations defined at mtk_eem_platform.c
 ***************************************************
 */
extern struct eem_det_ops gpu_det_ops;
extern struct eem_det_ops cpu_det_ops;
extern struct eem_det_ops cci_det_ops;

extern int get_volt_cpu(struct eem_det *det);
extern int set_volt_cpu(struct eem_det *det);
extern void restore_default_volt_cpu(struct eem_det *det);
extern void get_freq_table_cpu(struct eem_det *det);
extern void get_orig_volt_table_cpu(struct eem_det *det);
extern int get_volt_gpu(struct eem_det *det);
extern int set_volt_gpu(struct eem_det *det);
extern void restore_default_volt_gpu(struct eem_det *det);
extern void get_freq_table_gpu(struct eem_det *det);
extern void get_orig_volt_table_gpu(struct eem_det *det);


/*********************************************
 *extern operations defined at mtk_eem.c
 *********************************************
 */
extern void base_ops_enable(struct eem_det *det, int reason);
extern void base_ops_disable(struct eem_det *det, int reason);
extern void base_ops_disable_locked(struct eem_det *det, int reason);
extern void base_ops_switch_bank(struct eem_det *det, enum eem_phase phase);

extern int base_ops_init01(struct eem_det *det);
extern int base_ops_init02(struct eem_det *det);
extern int base_ops_mon_mode(struct eem_det *det);

extern int base_ops_get_status(struct eem_det *det);
extern void base_ops_dump_status(struct eem_det *det);

extern void base_ops_set_phase(struct eem_det *det, enum eem_phase phase);
extern int base_ops_get_temp(struct eem_det *det);
extern int base_ops_get_volt(struct eem_det *det);
extern int base_ops_set_volt(struct eem_det *det);
extern void base_ops_restore_default_volt(struct eem_det *det);
extern void base_ops_get_freq_table(struct eem_det *det);
extern void base_ops_get_orig_volt_table(struct eem_det *det);
#endif

