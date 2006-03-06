/*
    		 Crown Copyright (c) 1997
    
    This TenDRA(r) Computer Program is subject to Copyright
    owned by the United Kingdom Secretary of State for Defence
    acting through the Defence Evaluation and Research Agency
    (DERA).  It is made available to Recipients with a
    royalty-free licence for its use, reproduction, transfer
    to other parties and amendment for any purpose not excluding
    product development provided that any such use et cetera
    shall be deemed to be acceptance of the following conditions:-
    
        (1) Its Recipients shall ensure that this Notice is
        reproduced upon any copies or amended versions of it;
    
        (2) Any amended version of it shall be clearly marked to
        show both the nature of and the organisation responsible
        for the relevant amendment or amendments;
    
        (3) Its onward transfer from a recipient to another
        party shall be deemed to be that party's acceptance of
        these conditions;
    
        (4) DERA gives no warranty or assurance as to its
        quality or suitability for any purpose and DERA accepts
        no liability whatsoever in relation to any use to which
        it may be put.
*/


#ifndef ENC_NOS_INCLUDED
#define ENC_NOS_INCLUDED


/* AUTOMATICALLY GENERATED BY make_tdf VERSION 2.0 FROM TDF 4.0 */

#define major_version				4
#define minor_version				0

#define s_tdfint				0
#define s_tdfbool				1
#define s_tdfstring				2
#define s_bitstream				3
#define s_tdfident				4
#define s_bytestream				5

#define s_access				6
#define e_access_apply_token			1
#define e_access_cond				2
#define e_add_accesses				3
#define e_constant				4
#define e_long_jump_access			5
#define e_no_other_read				6
#define e_no_other_write			7
#define e_out_par				8
#define e_preserve				9
#define e_register				10
#define e_standard_access			11
#define e_used_as_volatile			12
#define e_visible				13

#define s_al_tag				7
#define e_al_tag_apply_token			2
#define e_make_al_tag				1

#define s_al_tagdef				8
#define e_make_al_tagdef			1

#define s_al_tagdef_props			9

#define s_alignment				10
#define e_alignment_apply_token			1
#define e_alignment_cond			2
#define e_alignment				3
#define e_alloca_alignment			4
#define e_callees_alignment			5
#define e_callers_alignment			6
#define e_code_alignment			7
#define e_locals_alignment			8
#define e_obtain_al_tag				9
#define e_parameter_alignment			10
#define e_unite_alignments			11
#define e_var_param_alignment			12

#define s_bitfield_variety			11
#define e_bfvar_apply_token			1
#define e_bfvar_cond				2
#define e_bfvar_bits				3

#define s_bool					12
#define e_bool_apply_token			1
#define e_bool_cond				2
#define e_false					3
#define e_true					4

#define s_callees				13
#define e_make_callee_list			1
#define e_make_dynamic_callees			2
#define e_same_callees				3

#define s_capsule				14

#define s_capsule_link				15

#define s_caselim				16

#define s_diag_descriptor			17
#define e_diag_desc_id				1
#define e_diag_desc_struct			2
#define e_diag_desc_typedef			3

#define s_diag_field				18

#define s_diag_tag				19
#define e_make_diag_tag				1

#define s_diag_tagdef				20
#define e_make_diag_tagdef			1

#define s_diag_tq				21
#define e_add_diag_const			1
#define e_add_diag_volatile			2
#define e_diag_tq_null				3

#define s_diag_type				22
#define e_diag_type_apply_token			1
#define e_diag_array				2
#define e_diag_bitfield				3
#define e_diag_enum				4
#define e_diag_floating_variety			5
#define e_diag_loc				6
#define e_diag_proc				7
#define e_diag_ptr				8
#define e_diag_struct				9
#define e_diag_type_null			10
#define e_diag_union				11
#define e_diag_variety				12
#define e_use_diag_tag				13

#define s_diag_type_unit			23

#define s_diag_unit				24

#define s_enum_values				25

#define s_error_code				26
#define e_nil_access				1
#define e_overflow				2
#define e_stack_overflow			3

#define s_error_treatment			27
#define e_errt_apply_token			1
#define e_errt_cond				2
#define e_continue				3
#define e_error_jump				4
#define e_trap					5
#define e_wrap					6
#define e_impossible				7

#define s_exp					28
#define e_exp_apply_token			1
#define e_exp_cond				2
#define e_abs					3
#define e_add_to_ptr				4
#define e_and					5
#define e_apply_proc				6
#define e_apply_general_proc			7
#define e_assign				8
#define e_assign_with_mode			9
#define e_bitfield_assign			10
#define e_bitfield_assign_with_mode		11
#define e_bitfield_contents			12
#define e_bitfield_contents_with_mode		13
#define e_case					14
#define e_change_bitfield_to_int		15
#define e_change_floating_variety		16
#define e_change_variety			17
#define e_change_int_to_bitfield		18
#define e_complex_conjugate			19
#define e_component				20
#define e_concat_nof				21
#define e_conditional				22
#define e_contents				23
#define e_contents_with_mode			24
#define e_current_env				25
#define e_div0					26
#define e_div1					27
#define e_div2					28
#define e_env_offset				29
#define e_env_size				30
#define e_fail_installer			31
#define e_float_int				32
#define e_floating_abs				33
#define e_floating_div				34
#define e_floating_minus			35
#define e_floating_maximum			36
#define e_floating_minimum			37
#define e_floating_mult				38
#define e_floating_negate			39
#define e_floating_plus				40
#define e_floating_power			41
#define e_floating_test				42
#define e_goto					43
#define e_goto_local_lv				44
#define e_identify				45
#define e_ignorable				46
#define e_imaginary_part			47
#define e_initial_value				48
#define e_integer_test				49
#define e_labelled				50
#define e_last_local				51
#define e_local_alloc				52
#define e_local_alloc_check			53
#define e_local_free				54
#define e_local_free_all			55
#define e_long_jump				56
#define e_make_complex				57
#define e_make_compound				58
#define e_make_floating				59
#define e_make_general_proc			60
#define e_make_int				61
#define e_make_local_lv				62
#define e_make_nof				63
#define e_make_nof_int				64
#define e_make_null_local_lv			65
#define e_make_null_proc			66
#define e_make_null_ptr				67
#define e_make_proc				68
#define e_make_stack_limit			116
#define e_make_top				69
#define e_make_value				70
#define e_maximum				71
#define e_minimum				72
#define e_minus					73
#define e_move_some				74
#define e_mult					75
#define e_n_copies				76
#define e_negate				77
#define e_not					78
#define e_obtain_tag				79
#define e_offset_add				80
#define e_offset_div				81
#define e_offset_div_by_int			82
#define e_offset_max				83
#define e_offset_mult				84
#define e_offset_negate				85
#define e_offset_pad				86
#define e_offset_subtract			87
#define e_offset_test				88
#define e_offset_zero				89
#define e_or					90
#define e_plus					91
#define e_pointer_test				92
#define e_power					93
#define e_proc_test				94
#define e_profile				95
#define e_real_part				96
#define e_rem0					97
#define e_rem1					98
#define e_rem2					99
#define e_repeat				100
#define e_return				101
#define e_return_to_label			102
#define e_round_with_mode			103
#define e_rotate_left				104
#define e_rotate_right				105
#define e_sequence				106
#define e_set_stack_limit			107
#define e_shape_offset				108
#define e_shift_left				109
#define e_shift_right				110
#define e_subtract_ptrs				111
#define e_tail_call				112
#define e_untidy_return				113
#define e_variable				114
#define e_xor					115

#define s_extern_link				29

#define s_external				30
#define e_string_extern				1
#define e_unique_extern				2
#define e_chain_extern				3

#define s_filename				31
#define e_filename_apply_token			1
#define e_make_filename				2

#define s_floating_variety			32
#define e_flvar_apply_token			1
#define e_flvar_cond				2
#define e_flvar_parms				3
#define e_complex_parms				4
#define e_float_of_complex			5
#define e_complex_of_float			6

#define s_group					33

#define s_label					34
#define e_label_apply_token			2
#define e_make_label				1

#define s_link					35

#define s_linkextern				36

#define s_linkinfo				37
#define e_static_name_def			1
#define e_make_comment				2
#define e_make_weak_defn			3
#define e_make_weak_symbol			4

#define s_linkinfo_props			38

#define s_links					39

#define s_nat					40
#define e_nat_apply_token			1
#define e_nat_cond				2
#define e_computed_nat				3
#define e_error_val				4
#define e_make_nat				5

#define s_ntest					41
#define e_ntest_apply_token			1
#define e_ntest_cond				2
#define e_equal					3
#define e_greater_than				4
#define e_greater_than_or_equal			5
#define e_less_than				6
#define e_less_than_or_equal			7
#define e_not_equal				8
#define e_not_greater_than			9
#define e_not_greater_than_or_equal		10
#define e_not_less_than				11
#define e_not_less_than_or_equal		12
#define e_less_than_or_greater_than		13
#define e_not_less_than_and_not_greater_than	14
#define e_comparable				15
#define e_not_comparable			16

#define s_otagexp				42

#define s_procprops				43
#define e_procprops_apply_token			1
#define e_procprops_cond			2
#define e_add_procprops				3
#define e_check_stack				4
#define e_inline				5
#define e_no_long_jump_dest			6
#define e_untidy				7
#define e_var_callees				8
#define e_var_callers				9

#define s_rounding_mode				44
#define e_rounding_mode_apply_token		1
#define e_rounding_mode_cond			2
#define e_round_as_state			3
#define e_to_nearest				4
#define e_toward_larger				5
#define e_toward_smaller			6
#define e_toward_zero				7

#define s_shape					45
#define e_shape_apply_token			1
#define e_shape_cond				2
#define e_bitfield				3
#define e_bottom				4
#define e_compound				5
#define e_floating				6
#define e_integer				7
#define e_nof					8
#define e_offset				9
#define e_pointer				10
#define e_proc					11
#define e_top					12

#define s_signed_nat				46
#define e_signed_nat_apply_token		1
#define e_signed_nat_cond			2
#define e_computed_signed_nat			3
#define e_make_signed_nat			4
#define e_snat_from_nat				5

#define s_sortname				47
#define e_access				1
#define e_al_tag				2
#define e_alignment_sort			3
#define e_bitfield_variety			4
#define e_bool					5
#define e_error_treatment			6
#define e_exp					7
#define e_floating_variety			8
#define e_foreign_sort				9
#define e_label					10
#define e_nat					11
#define e_ntest					12
#define e_procprops				13
#define e_rounding_mode				14
#define e_shape					15
#define e_signed_nat				16
#define e_string				17
#define e_tag					18
#define e_transfer_mode				19
#define e_token					20
#define e_variety				21

#define s_sourcemark				48
#define e_make_sourcemark			1

#define s_string				49
#define e_string_apply_token			1
#define e_string_cond				2
#define e_concat_string				3
#define e_make_string				4

#define s_tag					50
#define e_tag_apply_token			2
#define e_make_tag				1

#define s_tagacc				51

#define s_tagdec				52
#define e_make_id_tagdec			1
#define e_make_var_tagdec			2
#define e_common_tagdec				3

#define s_tagdec_props				53

#define s_tagdef				54
#define e_make_id_tagdef			1
#define e_make_var_tagdef			2
#define e_common_tagdef				3

#define s_tagdef_props				55

#define s_tagshacc				56

#define s_tokdec				57
#define e_make_tokdec				1

#define s_tokdec_props				58

#define s_tokdef				59
#define e_make_tokdef				1

#define s_tokdef_props				60

#define s_token					61
#define e_token_apply_token			1
#define e_make_tok				2
#define e_use_tokdef				3

#define s_token_defn				62
#define e_token_definition			1

#define s_tokformals				63

#define s_transfer_mode				64
#define e_transfer_mode_apply_token		1
#define e_transfer_mode_cond			2
#define e_add_modes				3
#define e_overlap				4
#define e_standard_transfer_mode		5
#define e_trap_on_nil				6
#define e_volatile				7
#define e_complete				8

#define s_unique				65

#define s_unit					66

#define s_variety				67
#define e_var_apply_token			1
#define e_var_cond				2
#define e_var_limits				3
#define e_var_width				4

#define s_version				68
#define e_make_version				1
#define e_user_info				2

#define s_version_props				69


#endif