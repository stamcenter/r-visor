.file	"util.S"
	.option nopic
	.attribute arch, "rv64i2p0_m2p0_a2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	
	.globl  rvisor_stub
	.extern rvisor_stub
	.type	rvisor_stub, @function

	.globl  rvisor_exit_binary
	.extern rvisor_exit_binary
	.type	rvisor_exit_binary, @function

	.globl  rvisor_instrument
	.extern rvisor_instrument
	.type	rvisor_instrument, @function

	.globl  rvisor_initialize_bin
	.extern rvisor_initialize_bin
	.type	rvisor_initialize_bin, @function



.globl	context_switch_trace_exit
.type	context_switch_trace_exit, @function

context_switch_trace_exit:
	# ld ra, 0(sp)
	ld t6, 0(sp)
	addi sp, sp, 32
	
	addi sp, sp, -272
	sd ra, 0(sp)
	sd sp, 8(sp)
	sd gp, 16(sp)
	sd tp, 24(sp)
	sd t0, 32(sp)
	sd t1, 40(sp)
	sd t2, 48(sp)
	sd s0, 56(sp)
	sd s1, 64(sp)
	sd a0, 72(sp)
	sd a1, 80(sp)
	sd a2, 88(sp)
	sd a3, 96(sp)
	sd a4, 104(sp)
	sd a5, 112(sp)
	sd a6, 120(sp)
	sd a7, 128(sp)
	sd s2, 136(sp)
	sd s3, 144(sp)
	sd s4, 152(sp)
	sd s5, 160(sp)
	sd s6, 168(sp)
	sd s7, 176(sp)
	sd s8, 184(sp)
	sd s9, 192(sp)
	sd s10, 200(sp)
	sd s11, 208(sp)
	sd t3, 216(sp)
	sd t4, 224(sp)
	sd t5, 232(sp)
	sd t6, 240(sp)

	mv a0, sp # Gain access to context within rvisor_stub
	li a1, 0 
	li a2, 0 # Flag for should Instrument
	ld a3, 248(sp) # Last Instruction
	call rvisor_stub

	# sd a0, 248(sp)
	mv t6, a0
	ld ra, 0(sp)
	
	ld sp, 8(sp)
	ld gp, 16(sp)
	ld tp, 24(sp)
	ld t0, 32(sp)
	ld t1, 40(sp)
	ld t2, 48(sp)
	ld s0, 56(sp)
	ld s1, 64(sp)
	ld a0, 72(sp)
	ld a1, 80(sp)
	ld a2, 88(sp)
	ld a3, 96(sp)
	ld a4, 104(sp)
	ld a5, 112(sp)
	ld a6, 120(sp)
	ld a7, 128(sp)
	ld s2, 136(sp)
	ld s3, 144(sp)
	ld s4, 152(sp)
	ld s5, 160(sp)
	ld s6, 168(sp)
	ld s7, 176(sp)
	ld s8, 184(sp)
	ld s9, 192(sp)
	ld s10, 200(sp)
	ld s11, 208(sp)
	ld t3, 216(sp)
	ld t4, 224(sp)
	ld t5, 232(sp)
	# ld t6, 240(sp)
	addi sp, sp, 272

    jalr x0, 0(t6)
	# jalr x0, -8(sp)
	# ret


.globl	context_switch_trace_exit_taken
.type	context_switch_trace_exit_taken, @function

context_switch_trace_exit_taken:
	# ld ra, 0(sp)
	ld t6, 0(sp)
	addi sp, sp, 32
	
	addi sp, sp, -272
	sd ra, 0(sp)
	sd sp, 8(sp)
	sd gp, 16(sp)
	sd tp, 24(sp)
	sd t0, 32(sp)
	sd t1, 40(sp)
	sd t2, 48(sp)
	sd s0, 56(sp)
	sd s1, 64(sp)
	sd a0, 72(sp)
	sd a1, 80(sp)
	sd a2, 88(sp)
	sd a3, 96(sp)
	sd a4, 104(sp)
	sd a5, 112(sp)
	sd a6, 120(sp)
	sd a7, 128(sp)
	sd s2, 136(sp)
	sd s3, 144(sp)
	sd s4, 152(sp)
	sd s5, 160(sp)
	sd s6, 168(sp)
	sd s7, 176(sp)
	sd s8, 184(sp)
	sd s9, 192(sp)
	sd s10, 200(sp)
	sd s11, 208(sp)
	sd t3, 216(sp)
	sd t4, 224(sp)
	sd t5, 232(sp)
	sd t6, 240(sp)

	mv a0, sp # Gain access to context within rvisor_stub
	li a1, 1 
	li a2, 0 # Flag for should Instrument
	ld a3, 248(sp) # Last Instruction
	call rvisor_stub

	# sd a0, 248(sp)
	mv t6, a0
	ld ra, 0(sp)
	
	ld sp, 8(sp)
	ld gp, 16(sp)
	ld tp, 24(sp)
	ld t0, 32(sp)
	ld t1, 40(sp)
	ld t2, 48(sp)
	ld s0, 56(sp)
	ld s1, 64(sp)
	ld a0, 72(sp)
	ld a1, 80(sp)
	ld a2, 88(sp)
	ld a3, 96(sp)
	ld a4, 104(sp)
	ld a5, 112(sp)
	ld a6, 120(sp)
	ld a7, 128(sp)
	ld s2, 136(sp)
	ld s3, 144(sp)
	ld s4, 152(sp)
	ld s5, 160(sp)
	ld s6, 168(sp)
	ld s7, 176(sp)
	ld s8, 184(sp)
	ld s9, 192(sp)
	ld s10, 200(sp)
	ld s11, 208(sp)
	ld t3, 216(sp)
	ld t4, 224(sp)
	ld t5, 232(sp)
	# ld t6, 240(sp)
	addi sp, sp, 272

    jalr x0, 0(t6)
	# jalr x0, -8(sp)
	# ret


.globl	context_switch
.type	context_switch, @function

context_switch:
	# ld ra, 0(sp)
	ld t6, 0(sp)
	addi sp, sp, 32
	
	addi sp, sp, -272
	sd ra, 0(sp)
	sd sp, 8(sp)
	sd gp, 16(sp)
	sd tp, 24(sp)
	sd t0, 32(sp)
	sd t1, 40(sp)
	sd t2, 48(sp)
	sd s0, 56(sp)
	sd s1, 64(sp)
	sd a0, 72(sp)
	sd a1, 80(sp)
	sd a2, 88(sp)
	sd a3, 96(sp)
	sd a4, 104(sp)
	sd a5, 112(sp)
	sd a6, 120(sp)
	sd a7, 128(sp)
	sd s2, 136(sp)
	sd s3, 144(sp)
	sd s4, 152(sp)
	sd s5, 160(sp)
	sd s6, 168(sp)
	sd s7, 176(sp)
	sd s8, 184(sp)
	sd s9, 192(sp)
	sd s10, 200(sp)
	sd s11, 208(sp)
	sd t3, 216(sp)
	sd t4, 224(sp)
	sd t5, 232(sp)
	sd t6, 240(sp)

	mv a0, sp # Gain access to context within rvisor_stub
	li a1, 0 
	li a2, 0 # Flag for should Instrument
	li a3, 0 # Flag for should Instrument
	call rvisor_stub

	# sd a0, 248(sp)
	mv t6, a0
	ld ra, 0(sp)
	
	ld sp, 8(sp)
	ld gp, 16(sp)
	ld tp, 24(sp)
	ld t0, 32(sp)
	ld t1, 40(sp)
	ld t2, 48(sp)
	ld s0, 56(sp)
	ld s1, 64(sp)
	ld a0, 72(sp)
	ld a1, 80(sp)
	ld a2, 88(sp)
	ld a3, 96(sp)
	ld a4, 104(sp)
	ld a5, 112(sp)
	ld a6, 120(sp)
	ld a7, 128(sp)
	ld s2, 136(sp)
	ld s3, 144(sp)
	ld s4, 152(sp)
	ld s5, 160(sp)
	ld s6, 168(sp)
	ld s7, 176(sp)
	ld s8, 184(sp)
	ld s9, 192(sp)
	ld s10, 200(sp)
	ld s11, 208(sp)
	ld t3, 216(sp)
	ld t4, 224(sp)
	ld t5, 232(sp)
	# ld t6, 240(sp)
	addi sp, sp, 272

    jalr x0, 0(t6)
	# jalr x0, -8(sp)
	# ret


.globl	context_switch_taken
.type	context_switch_taken, @function

context_switch_taken:
	# ld ra, 0(sp)
	ld t6, 0(sp)
	addi sp, sp, 32
	
	addi sp, sp, -272
	sd ra, 0(sp)
	sd sp, 8(sp)
	sd gp, 16(sp)
	sd tp, 24(sp)
	sd t0, 32(sp)
	sd t1, 40(sp)
	sd t2, 48(sp)
	sd s0, 56(sp)
	sd s1, 64(sp)
	sd a0, 72(sp)
	sd a1, 80(sp)
	sd a2, 88(sp)
	sd a3, 96(sp)
	sd a4, 104(sp)
	sd a5, 112(sp)
	sd a6, 120(sp)
	sd a7, 128(sp)
	sd s2, 136(sp)
	sd s3, 144(sp)
	sd s4, 152(sp)
	sd s5, 160(sp)
	sd s6, 168(sp)
	sd s7, 176(sp)
	sd s8, 184(sp)
	sd s9, 192(sp)
	sd s10, 200(sp)
	sd s11, 208(sp)
	sd t3, 216(sp)
	sd t4, 224(sp)
	sd t5, 232(sp)
	sd t6, 240(sp)

	mv a0, sp # Gain access to context within rvisor_stub
	li a1, 1 # Flag for taken branch
	li a2, 0 # Flag for should Instrument
	li a3, 0 # Flag for should Instrument
	call rvisor_stub

	# sd a0, 248(sp)
	mv t6, a0
	ld ra, 0(sp)
	
	ld sp, 8(sp)
	ld gp, 16(sp)
	ld tp, 24(sp)
	ld t0, 32(sp)
	ld t1, 40(sp)
	ld t2, 48(sp)
	ld s0, 56(sp)
	ld s1, 64(sp)
	ld a0, 72(sp)
	ld a1, 80(sp)
	ld a2, 88(sp)
	ld a3, 96(sp)
	ld a4, 104(sp)
	ld a5, 112(sp)
	ld a6, 120(sp)
	ld a7, 128(sp)
	ld s2, 136(sp)
	ld s3, 144(sp)
	ld s4, 152(sp)
	ld s5, 160(sp)
	ld s6, 168(sp)
	ld s7, 176(sp)
	ld s8, 184(sp)
	ld s9, 192(sp)
	ld s10, 200(sp)
	ld s11, 208(sp)
	ld t3, 216(sp)
	ld t4, 224(sp)
	ld t5, 232(sp)
	# ld t6, 240(sp)
	addi sp, sp, 272

    jalr x0, 0(t6)
	# jalr x0, -8(sp)
	# ret

.globl	context_switch_instrument
.type	context_switch_instrument, @function
context_switch_instrument:
	# ld ra, 0(sp)
	ld t6, 0(sp)
	addi sp, sp, 32
	
	addi sp, sp, -272
	sd ra, 0(sp)
	sd sp, 8(sp)
	sd gp, 16(sp)
	sd tp, 24(sp)
	sd t0, 32(sp)
	sd t1, 40(sp)
	sd t2, 48(sp)
	sd s0, 56(sp)
	sd s1, 64(sp)
	sd a0, 72(sp)
	sd a1, 80(sp)
	sd a2, 88(sp)
	sd a3, 96(sp)
	sd a4, 104(sp)
	sd a5, 112(sp)
	sd a6, 120(sp)
	sd a7, 128(sp)
	sd s2, 136(sp)
	sd s3, 144(sp)
	sd s4, 152(sp)
	sd s5, 160(sp)
	sd s6, 168(sp)
	sd s7, 176(sp)
	sd s8, 184(sp)
	sd s9, 192(sp)
	sd s10, 200(sp)
	sd s11, 208(sp)
	sd t3, 216(sp)
	sd t4, 224(sp)
	sd t5, 232(sp)
	sd t6, 240(sp)

	mv a0, sp # Gain access to context within rvisor_stub
	li a1, 1 # Flag for taken branch
	li a2, 1 # Flag for should Instrument
	li a3, 0 # Flag for should Instrument
	call rvisor_instrument

	# sd a0, 248(sp)
	mv t6, a0
	ld ra, 0(sp)
	
	ld sp, 8(sp)
	ld gp, 16(sp)
	ld tp, 24(sp)
	ld t0, 32(sp)
	ld t1, 40(sp)
	ld t2, 48(sp)
	ld s0, 56(sp)
	ld s1, 64(sp)
	ld a0, 72(sp)
	ld a1, 80(sp)
	ld a2, 88(sp)
	ld a3, 96(sp)
	ld a4, 104(sp)
	ld a5, 112(sp)
	ld a6, 120(sp)
	ld a7, 128(sp)
	ld s2, 136(sp)
	ld s3, 144(sp)
	ld s4, 152(sp)
	ld s5, 160(sp)
	ld s6, 168(sp)
	ld s7, 176(sp)
	ld s8, 184(sp)
	ld s9, 192(sp)
	ld s10, 200(sp)
	ld s11, 208(sp)
	ld t3, 216(sp)
	ld t4, 224(sp)
	ld t5, 232(sp)
	# ld t6, 240(sp)
	addi sp, sp, 272

    jalr x0, 0(t6)
	# jalr x0, -8(sp)
	# ret

.globl	init_switch
.type	init_switch, @function
init_switch:
	# ld ra, 0(sp)
	# ld t6, 0(sp)
	# addi sp, sp, 32
	
	# addi sp, sp, -24
	# sd ra, 0(sp)
	# sd sp, 8(sp)
	# sd gp, 16(sp)
	# sd tp, 24(sp)
	# sd t0, 32(sp)
	# sd t1, 40(sp)
	# sd t2, 48(sp)
	# sd s0, 56(sp)
	# sd s1, 64(sp)
	# sd a0, 72(sp)
	# sd a1, 80(sp)
	# sd a2, 88(sp)
	# sd a3, 96(sp)
	# sd a4, 104(sp)
	# sd a5, 112(sp)
	# sd a6, 120(sp)
	# sd a7, 128(sp)
	# sd s2, 136(sp)
	# sd s3, 144(sp)
	# sd s4, 152(sp)
	# sd s5, 160(sp)
	# sd s6, 168(sp)
	# sd s7, 176(sp)
	# sd s8, 184(sp)
	# sd s9, 192(sp)
	# sd s10, 200(sp)
	# sd s11, 208(sp)
	# sd t3, 216(sp)
	# sd t4, 224(sp)
	# sd t5, 232(sp)
	# sd t6, 240(sp)
	li a0, 4096
	sub sp, sp, a0
	mv a0, sp # Gain access to context within rvisor_stub
	li a1, 1 # Flag for taken branch
	li a2, 1 # Flag for should Instrument
	li a3, 0 # Flag for should Instrument
	call rvisor_initialize_bin

	# sd a0, 248(sp)
	mv t6, a0

	addi a0, zero, 0
	addi a1, zero, 0
	addi a2, zero, 0
	addi a3, zero, 0
	addi a4, zero, 0
	addi a5, zero, 0
	addi a6, zero, 0
	addi a7, zero, 0
	addi s0, zero, 0
	addi s1, zero, 0
	addi s2, zero, 0
	addi s3, zero, 0
	addi s4, zero, 0
	addi s5, zero, 0
	addi s6, zero, 0
	addi s7, zero, 0
	addi s8, zero, 0
	addi s9, zero, 0
	addi s10, zero, 0
	addi s11, zero, 0
	addi t0, zero, 0
	addi t1, zero, 0
	addi t2, zero, 0
	addi t3, zero, 0
	addi t4, zero, 0
	addi t5, zero, 0

    jalr x0, 0(t6)
	# jalr x0, -8(sp)
	# ret

.globl	inline_save
.type	inline_save, @function

inline_save:
	addi sp, sp, -32
	sd t6, 0(sp)

.globl	inline_save_T5
.type	inline_save_T5, @function

inline_save_T5:
	addi sp, sp, -32
	sd t5, 0(sp)

.globl	inline_load
.type	inline_load, @function

inline_load:
	ld t6, 0(sp)
	addi sp, sp, 32

.globl	inline_load_T5
.type	inline_load_T5, @function
inline_load_T5:
	ld t5, 0(sp)
	addi sp, sp, 32

restore_ra:
	ld ra, -256(sp)


.globl	restore_scratch
.type	restore_scratch, @function

restore_scratch:
	ld t6, -32(sp)


.globl	exit_context_switch
.type	exit_context_switch, @function
exit_context_switch:
	ld t6, 0(sp)
	addi sp, sp, 32
	
	addi sp, sp, -272
	sd ra, 0(sp)
	sd sp, 8(sp)
	sd gp, 16(sp)
	sd tp, 24(sp)
	sd t0, 32(sp)
	sd t1, 40(sp)
	sd t2, 48(sp)
	sd s0, 56(sp)
	sd s1, 64(sp)
	sd a0, 72(sp)
	sd a1, 80(sp)
	sd a2, 88(sp)
	sd a3, 96(sp)
	sd a4, 104(sp)
	sd a5, 112(sp)
	sd a6, 120(sp)
	sd a7, 128(sp)
	sd s2, 136(sp)
	sd s3, 144(sp)
	sd s4, 152(sp)
	sd s5, 160(sp)
	sd s6, 168(sp)
	sd s7, 176(sp)
	sd s8, 184(sp)
	sd s9, 192(sp)
	sd s10, 200(sp)
	sd s11, 208(sp)
	sd t3, 216(sp)
	sd t4, 224(sp)
	sd t5, 232(sp)
	sd t6, 240(sp)

	mv a0, sp # Gain access to context within rvisor_stub
	call rvisor_exit_binary
	
