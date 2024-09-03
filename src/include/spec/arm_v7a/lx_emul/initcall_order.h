/*
 * \brief  Array defining order of Linux Kernel initcalls
 * \author Automatically generated file - do no edit
 * \date   2024-09-03
 */

#pragma once

static const char * lx_emul_initcall_order[] = {
	"__initcall_allocate_overflow_stacksearly",
	"__initcall_start",
	"__initcall_init_static_idmapearly",
	"__initcall_spawn_ksoftirqdearly",
	"__initcall_init_signal_sysctlsearly",
	"__initcall_init_umh_sysctlsearly",
	"__initcall_migration_initearly",
	"__initcall_srcu_bootup_announceearly",
	"__initcall_rcu_sysrq_initearly",
	"__initcall_check_cpu_stall_initearly",
	"__initcall_rcu_spawn_gp_kthreadearly",
	"__initcall_cpu_stop_initearly",
	"__initcall_irq_work_init_threadsearly",
	"__initcall_init_zero_pfnearly",
	"__initcall_init_fs_inode_sysctlsearly",
	"__initcall_init_fs_locks_sysctlsearly",
	"__initcall_init_fs_sysctlsearly",
	"__initcall_dummy_timer_registerearly",
	"__initcall_init_mmap_min_addr0",
	"__initcall_inet_frag_wq_init0",
	"__initcall_ptrace_break_init1",
	"__initcall_twd_clk_init1",
	"__initcall_v6_userpage_init1",
	"__initcall_wq_sysfs_init1",
	"__initcall_ksysfs_init1",
	"__initcall_rcu_set_runtime_mode1",
	"__initcall_init_jiffies_clocksource1",
	"__initcall_cma_init_reserved_areas1",
	"__initcall_filelock_init1",
	"__initcall_init_script_binfmt1",
	"__initcall_init_elf_binfmt1",
	"__initcall_pinctrl_init1",
	"__initcall_gpiolib_dev_init1",
	"__initcall_fsl_guts_init1",
	"__initcall_regulator_init1",
	"__initcall_soc_bus_register1",
	"__initcall_free_raw_capacity1",
	"__initcall_sock_init1",
	"__initcall_net_inuse_init1",
	"__initcall_net_defaults_init1",
	"__initcall_init_default_flow_dissectors1",
	"__initcall_netlink_proto_init1",
	"__initcall_genl_init1",
	"__initcall_atomic_pool_init2",
	"__initcall_imx_mmdc_init2",
	"__initcall_irq_sysfs_init2",
	"__initcall_bdi_class_init2",
	"__initcall_mm_sysfs_init2",
	"__initcall_init_per_zone_wmark_min2",
	"__initcall_anatop_regulator_init2",
	"__initcall_tty_class_init2",
	"__initcall_vtconsole_class_init2",
	"__initcall_devlink_class_init2",
	"__initcall_software_node_init2",
	"__initcall_regmap_initcall2",
	"__initcall_sram_init2",
	"__initcall_syscon_init2",
	"__initcall_spi_init2",
	"__initcall_mxs_phy_module_init2",
	"__initcall_i2c_init2",
	"__initcall_kobject_uevent_init2",
	"__initcall_gate_vma_init3",
	"__initcall_customize_machine3",
	"__initcall_vdso_init3",
	"__initcall_exceptions_init3",
	"__initcall_imx53_pinctrl_init3",
	"__initcall_imx6q_pinctrl_init3",
	"__initcall_imx6dl_pinctrl_init3",
	"__initcall_imx6sx_pinctrl_init3",
	"__initcall_imx7d_pinctrl_init3",
	"__initcall_serial_base_init3",
	"__initcall_of_platform_default_populate_init3s",
	"__initcall_topology_init4",
	"__initcall_uid_cache_init4",
	"__initcall_param_sysfs_init4",
	"__initcall_user_namespace_sysctl_init4",
	"__initcall_oom_init4",
	"__initcall_default_bdi_init4",
	"__initcall_percpu_enable_async4",
	"__initcall_kcompactd_init4",
	"__initcall_init_reserve_notifier4",
	"__initcall_init_admin_reserve4",
	"__initcall_init_user_reserve4",
	"__initcall_gpio_mxc_init4",
	"__initcall_regulator_fixed_voltage_init4",
	"__initcall_gpio_regulator_init4",
	"__initcall_misc_init4",
	"__initcall_register_cpu_capacity_sysctl4",
	"__initcall_phy_init4",
	"__initcall_usb_common_init4",
	"__initcall_ulpi_init4",
	"__initcall_usb_init4",
	"__initcall_usb_phy_generic_init4",
	"__initcall_usb_udc_init4",
	"__initcall_usb_roles_init4",
	"__initcall_serio_init4",
	"__initcall_input_init4",
	"__initcall_i2c_gpio_init4",
	"__initcall_i2c_adap_imx_init4",
	"__initcall_pps_init4",
	"__initcall_ptp_init4",
	"__initcall_mmc_init4",
	"__initcall_mux_init4",
	"__initcall_proto_init4",
	"__initcall_net_dev_init4",
	"__initcall_neigh_init4",
	"__initcall_fib_notifier_init4",
	"__initcall_netdev_genl_init4",
	"__initcall_ethnl_init4",
	"__initcall_nexthop_init4",
	"__initcall_vsprintf_init_hashval4",
	"__initcall_proc_cpu_init5",
	"__initcall_alignment_init5",
	"__initcall_iomem_init_inode5",
	"__initcall_clocksource_done_booting5",
	"__initcall_init_fs_stat_sysctls5",
	"__initcall_init_fs_exec_sysctls5",
	"__initcall_init_pipe_fs5",
	"__initcall_init_fs_namei_sysctls5",
	"__initcall_init_fs_dcache_sysctls5",
	"__initcall_init_fs_namespace_sysctls5",
	"__initcall_anon_inode_init5",
	"__initcall_proc_locks_init5",
	"__initcall_proc_cmdline_init5",
	"__initcall_proc_consoles_init5",
	"__initcall_proc_cpuinfo_init5",
	"__initcall_proc_devices_init5",
	"__initcall_proc_interrupts_init5",
	"__initcall_proc_loadavg_init5",
	"__initcall_proc_meminfo_init5",
	"__initcall_proc_stat_init5",
	"__initcall_proc_uptime_init5",
	"__initcall_proc_version_init5",
	"__initcall_proc_softirqs_init5",
	"__initcall_proc_kmsg_init5",
	"__initcall_proc_page_init5",
	"__initcall_init_ramfs_fs5",
	"__initcall_chr_dev_init5",
	"__initcall_sysctl_core_init5",
	"__initcall_eth_offload_init5",
	"__initcall_inet_init5",
	"__initcall_ipv4_offload_init5",
	"__initcall_af_unix_init5",
	"__initcall_ipv6_offload_init5",
	"__initcall_populate_rootfsrootfs",
	"__initcall_imx_src_driver_init6",
	"__initcall_proc_execdomains_init6",
	"__initcall_ioresources_init6",
	"__initcall_irq_gc_init_ops6",
	"__initcall_timer_sysctl_init6",
	"__initcall_timekeeping_init_ops6",
	"__initcall_init_clocksource_sysfs6",
	"__initcall_init_timer_list_procfs6",
	"__initcall_alarmtimer_init6",
	"__initcall_init_posix_timers6",
	"__initcall_clockevents_init_sysfs6",
	"__initcall_sched_clock_syscore_init6",
	"__initcall_utsname_sysctl_init6",
	"__initcall_kswapd_init6",
	"__initcall_mm_compute_batch_init6",
	"__initcall_workingset_init6",
	"__initcall_proc_vmalloc_init6",
	"__initcall_fcntl_init6",
	"__initcall_proc_filesystems_init6",
	"__initcall_start_dirtytime_writeback6",
	"__initcall_init_devpts_fs6",
	"__initcall_blake2s_mod_init6",
	"__initcall_percpu_counter_startup6",
	"__initcall_imx_irqsteer_driver_init6",
	"__initcall_imx_intmux_driver_init6",
	"__initcall_imx_mu_msi_driver_init6",
	"__initcall_simple_pm_bus_driver_init6",
	"__initcall_gen_74x164_driver_init6",
	"__initcall_of_fixed_factor_clk_driver_init6",
	"__initcall_of_fixed_clk_driver_init6",
	"__initcall_gpio_clk_driver_init6",
	"__initcall_imx_soc_device_init6",
	"__initcall_imx7_reset_driver_init6",
	"__initcall_n_null_init6",
	"__initcall_pty_init6",
	"__initcall_imx_uart_init6",
	"__initcall_random_sysctls_init6",
	"__initcall_topology_sysfs_init6",
	"__initcall_cacheinfo_sysfs_init6",
	"__initcall_spi_gpio_driver_init6",
	"__initcall_spi_imx_driver_init6",
	"__initcall_blackhole_netdev_init6",
	"__initcall_phylink_init6",
	"__initcall_phy_module_init6",
	"__initcall_phy_module_init6",
	"__initcall_fixed_mdio_bus_init6",
	"__initcall_mdio_mux_multiplexer_driver_init6",
	"__initcall_fec_driver_init6",
	"__initcall_asix_driver_init6",
	"__initcall_ax88179_178a_driver_init6",
	"__initcall_cdc_driver_init6",
	"__initcall_net1080_driver_init6",
	"__initcall_cdc_subset_driver_init6",
	"__initcall_zaurus_driver_init6",
	"__initcall_usbnet_init6",
	"__initcall_cdc_ncm_driver_init6",
	"__initcall_r8153_ecm_driver_init6",
	"__initcall_ehci_hcd_init6",
	"__initcall_ohci_hcd_mod_init6",
	"__initcall_ci_hdrc_platform_register6",
	"__initcall_ci_hdrc_usb2_driver_init6",
	"__initcall_ci_hdrc_msm_driver_init6",
	"__initcall_usbmisc_imx_driver_init6",
	"__initcall_ci_hdrc_imx_driver_init6",
	"__initcall_tegra_usb_driver_init6",
	"__initcall_serport_init6",
	"__initcall_atkbd_init6",
	"__initcall_lpi2c_imx_driver_init6",
	"__initcall_mmc_pwrseq_simple_driver_init6",
	"__initcall_mmc_pwrseq_emmc_driver_init6",
	"__initcall_mxcmci_driver_init6",
	"__initcall_sdhci_drv_init6",
	"__initcall_sdhci_pltfm_drv_init6",
	"__initcall_sdhci_esdhc_imx_driver_init6",
	"__initcall_sdhci_esdhc_driver_init6",
	"__initcall_hid_init6",
	"__initcall_hid_generic_init6",
	"__initcall_hid_init6",
	"__initcall_extcon_class_init6",
	"__initcall_mux_mmio_driver_init6",
	"__initcall_sock_diag_init6",
	"__initcall_gre_offload_init6",
	"__initcall_sysctl_ipv4_init6",
	"__initcall_tunnel4_init6",
	"__initcall_inet_diag_init6",
	"__initcall_tcp_diag_init6",
	"__initcall_cubictcp_register6",
	"__initcall_inet6_init6",
	"__initcall_sit_init6",
	"__initcall_packet_init6",
	"__initcall_kernel_do_mounts_initrd_sysctls_init7",
	"__initcall_init_machine_late7",
	"__initcall_swp_emulation_init7",
	"__initcall_kernel_panic_sysfs_init7",
	"__initcall_kernel_panic_sysctls_init7",
	"__initcall_kernel_exit_sysfs_init7",
	"__initcall_kernel_exit_sysctls_init7",
	"__initcall_param_sysfs_builtin_init7",
	"__initcall_reboot_ksysfs_init7",
	"__initcall_sched_core_sysctl_init7",
	"__initcall_sched_fair_sysctl_init7",
	"__initcall_sched_dl_sysctl_init7",
	"__initcall_sched_rt_sysctl_init7",
	"__initcall_printk_late_init7",
	"__initcall_check_early_ioremap_leak7",
	"__initcall_sync_state_resume_initcall7",
	"__initcall_deferred_probe_initcall7",
	"__initcall_of_fdt_raw_init7",
	"__initcall_init_subsystem7",
	"__initcall_bpf_kfunc_init7",
	"__initcall_xdp_metadata_init7",
	"__initcall_tcp_congestion_default7",
	"__initcall_ip_auto_config7",
	"__initcall_clk_disable_unused7s",
	"__initcall_imx_clk_disable_uart7s",
	"__initcall_regulator_init_complete7s",
	"__initcall_of_platform_sync_state_init7s",
	"__initcall_con_initcon",
	"__initcall_end",
	"END_OF_INITCALL_ORDER_ARRAY_DUMMY_ENTRY"
};
