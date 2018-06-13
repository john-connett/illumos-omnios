/*
* CDDL HEADER START
*
* The contents of this file are subject to the terms of the
* Common Development and Distribution License, v.1,  (the "License").
* You may not use this file except in compliance with the License.
*
* You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
* or http://opensource.org/licenses/CDDL-1.0.
* See the License for the specific language governing permissions
* and limitations under the License.
*
* When distributing Covered Code, include this CDDL HEADER in each
* file and include the License file at usr/src/OPENSOLARIS.LICENSE.
* If applicable, add the following below this CDDL HEADER, with the
* fields enclosed by brackets "[]" replaced with your own identifying
* information: Portions Copyright [yyyy] [name of copyright owner]
*
* CDDL HEADER END
*/

/*
* Copyright 2014-2017 Cavium, Inc. 
* The contents of this file are subject to the terms of the Common Development 
* and Distribution License, v.1,  (the "License").

* You may not use this file except in compliance with the License.

* You can obtain a copy of the License at available 
* at http://opensource.org/licenses/CDDL-1.0

* See the License for the specific language governing permissions and 
* limitations under the License.
*/

#ifndef __ECORE_ISCSI_API_H__
#define __ECORE_ISCSI_API_H__

#ifndef __EXTRACT__LINUX__
typedef enum _ecore_status_t (*iscsi_event_cb_t)(void *context,
						 u8 fw_event_code,
						 void *fw_handle);
#endif

struct ecore_iscsi_conn {
	osal_list_entry_t	list_entry;
	bool			free_on_delete;

	u16			conn_id;
	u32			icid;
	u32			fw_cid;

	u8			layer_code;
	u8			offl_flags;
	u8			connect_mode;
	u32			initial_ack;
	dma_addr_t		sq_pbl_addr;
	struct ecore_chain	r2tq;
	struct ecore_chain	xhq;
	struct ecore_chain	uhq;

	struct tcp_upload_params *tcp_upload_params_virt_addr;
	dma_addr_t		tcp_upload_params_phys_addr;
	struct scsi_terminate_extra_params *queue_cnts_virt_addr;
	dma_addr_t		queue_cnts_phys_addr;
	dma_addr_t		syn_phy_addr;

	u16			syn_ip_payload_length;
	u8			local_mac[6];
	u8			remote_mac[6];
	u16			vlan_id;
	u8			tcp_flags;
	u8			ip_version;
	u32			remote_ip[4];
	u32			local_ip[4];
	u8			ka_max_probe_cnt;
	u8			dup_ack_theshold;
	u32			rcv_next;
	u32			snd_una;
	u32			snd_next;
	u32			snd_max;
	u32			snd_wnd;
	u32			rcv_wnd;
	u32			snd_wl1;
	u32			cwnd;
	u32			ss_thresh;
	u16			srtt;
	u16			rtt_var;
	u32			ts_recent;
	u32			ts_recent_age;
	u32			total_rt;
	u32			ka_timeout_delta;
	u32			rt_timeout_delta;
	u8			dup_ack_cnt;
	u8			snd_wnd_probe_cnt;
	u8			ka_probe_cnt;
	u8			rt_cnt;
	u32			flow_label;
	u32			ka_timeout;
	u32			ka_interval;
	u32			max_rt_time;
	u32			initial_rcv_wnd;
	u8			ttl;
	u8			tos_or_tc;
	u16			remote_port;
	u16			local_port;
	u16			mss;
	u8			snd_wnd_scale;
	u8			rcv_wnd_scale;
	u16			da_timeout_value;
	u8			ack_frequency;

	u8			update_flag;
#define	ECORE_ISCSI_CONN_HD_EN		0x01
#define	ECORE_ISCSI_CONN_DD_EN		0x02
#define	ECORE_ISCSI_CONN_INITIAL_R2T	0x04
#define	ECORE_ISCSI_CONN_IMMEDIATE_DATA	0x08

	u8			default_cq;
	u32			max_seq_size;
	u32			max_recv_pdu_length;
	u32			max_send_pdu_length;
	u32			first_seq_length;
	u32			exp_stat_sn;
	u32			stat_sn;
	u16			physical_q0;
	u16			physical_q1;
	u8			abortive_dsconnect;
};

#ifndef __EXTRACT__LINUX__
struct ecore_iscsi_stats
{
	u64 iscsi_rx_bytes_cnt;
	u64 iscsi_rx_packet_cnt;
	u64 iscsi_rx_new_ooo_isle_events_cnt;
	u32 iscsi_cmdq_threshold_cnt;
	u32 iscsi_rq_threshold_cnt;
	u32 iscsi_immq_threshold_cnt;

	u64 iscsi_rx_dropped_pdus_task_not_valid;

	u64 iscsi_rx_data_pdu_cnt;
	u64 iscsi_rx_r2t_pdu_cnt;
	u64 iscsi_rx_total_pdu_cnt;

	u64 iscsi_tx_go_to_slow_start_event_cnt;
	u64 iscsi_tx_fast_retransmit_event_cnt;

	u64 iscsi_tx_data_pdu_cnt;
	u64 iscsi_tx_r2t_pdu_cnt;
	u64 iscsi_tx_total_pdu_cnt;

	u64 iscsi_tx_bytes_cnt;
	u64 iscsi_tx_packet_cnt;
};
#endif

/**
 * @brief ecore_iscsi_acquire_connection - allocate resources, 
 *        provides connecion handle (CID)as out parameter.
 *
 * @param p_path
 * @param p_conn  partially initialized incoming container of 
 *                iSCSI connection data
 * @return enum _ecore_status_t
 */
enum _ecore_status_t
ecore_iscsi_acquire_connection(struct ecore_hwfn *p_hwfn,
			       struct ecore_iscsi_conn *p_in_conn,
			       struct ecore_iscsi_conn **p_out_conn);

void OSAL_IOMEM *ecore_iscsi_get_db_addr(struct ecore_hwfn *p_hwfn,
					 u32 cid);

void OSAL_IOMEM *ecore_iscsi_get_global_cmdq_cons(struct ecore_hwfn *p_hwfn,
						  u8 relative_q_id);

void OSAL_IOMEM *ecore_iscsi_get_primary_bdq_prod(struct ecore_hwfn *p_hwfn,
						  u8 bdq_id);

void OSAL_IOMEM *ecore_iscsi_get_secondary_bdq_prod(struct ecore_hwfn *p_hwfn,
						    u8 bdq_id);

/**
 * @brief ecore_iscsi_offload_connection - offload previously 
 *        allocated iSCSI connection
 *
 * @param p_path 
 * @param p_conn  container of iSCSI connection data
 *  
 * @return enum _ecore_status_t
 */
enum _ecore_status_t
ecore_iscsi_offload_connection(struct ecore_hwfn *p_hwfn,
			       struct ecore_iscsi_conn *p_conn);

/**
 * @brief ecore_iscsi_release_connection - deletes connecton 
 *        resources (incliding container of iSCSI connection
 *        data)
 *
 * @param p_path 
 * @param p_conn  container of iSCSI connection data
 */
void ecore_iscsi_release_connection(struct ecore_hwfn *p_hwfn,
				    struct ecore_iscsi_conn *p_conn);

/**
 * @brief ecore_iscsi_terminate_connection - destroys previously
 *        offloaded iSCSI connection
 *
 * @param p_path 
 * @param p_conn  container of iSCSI connection data
 *  
 * @return enum _ecore_status_t
 */
enum _ecore_status_t
ecore_iscsi_terminate_connection(struct ecore_hwfn *p_hwfn,
				 struct ecore_iscsi_conn *p_conn);


/**
 * @brief ecore_iscsi_update_connection - updates previously 
 *        offloaded iSCSI connection
 *
 *
 * @param p_path 
 * @param p_conn  container of iSCSI connection data
 *  
 * @return enum _ecore_status_t
 */
enum _ecore_status_t
ecore_iscsi_update_connection(struct ecore_hwfn *p_hwfn,
			      struct ecore_iscsi_conn *p_conn);

/**
 * @brief ecore_iscsi_mac_update_connection - updates remote MAC for previously
 *        offloaded iSCSI connection
 *
 *
 * @param p_path
 * @param p_conn  container of iSCSI connection data
 *
 * @return enum _ecore_status_t
 */
enum _ecore_status_t
ecore_iscsi_update_remote_mac(struct ecore_hwfn *p_hwfn,
			      struct ecore_iscsi_conn *p_conn);

/**
 * @brief ecore_iscsi_clear_connection_sq - clear SQ
 *        offloaded iSCSI connection
 *
 *
 * @param p_path
 * @param p_conn  container of iSCSI connection data
 *
 * @return enum _ecore_status_t
 */
enum _ecore_status_t
ecore_iscsi_clear_connection_sq(struct ecore_hwfn *p_hwfn,
				struct ecore_iscsi_conn *p_conn);

/**
 * @brief ecore_sp_iscsi_func_start
 *
 * This ramrod inits iSCSI functionality in FW
 *
 * @param p_path
 * @param comp_mode
 * @param comp_addr
 *
 * @return enum _ecore_status_t
 */
enum _ecore_status_t
ecore_sp_iscsi_func_start(struct ecore_hwfn *p_hwfn,
			  enum spq_mode comp_mode,
			  struct ecore_spq_comp_cb *p_comp_addr,
			  void *async_event_context,
			  iscsi_event_cb_t async_event_cb);

enum _ecore_status_t
ecore_sp_iscsi_func_stop(struct ecore_hwfn *p_hwfn,
			 enum spq_mode comp_mode,
			 struct ecore_spq_comp_cb *p_comp_addr);

enum _ecore_status_t
ecore_iscsi_get_stats(struct ecore_hwfn *p_hwfn,
		      struct ecore_iscsi_stats *stats);

#endif
