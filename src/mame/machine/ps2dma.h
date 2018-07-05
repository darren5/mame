// license:BSD-3-Clause
// copyright-holders:Ryan Holtz
/******************************************************************************
*
*   Sony Playstation 2 DMAC device skeleton
*
*   To Do:
*     Everything
*
*/

#ifndef MAME_MACHINE_PS2DMAC_H
#define MAME_MACHINE_PS2DMAC_H

#pragma once

#include "emu.h"

class ps2_dmac_device : public device_t, public device_execute_interface
{
public:
	template <typename T, typename U, typename V>
    ps2_dmac_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock, T &&ee_tag, U &&ram_tag, V &&sif_tag)
    	: ps2_dmac_device(mconfig, tag, owner, clock)
    {
		m_ee.set_tag(std::forward<T>(ee_tag));
		m_ram.set_tag(std::forward<U>(ram_tag));
		m_sif.set_tag(std::forward<V>(sif_tag));
	}

    ps2_dmac_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	enum channel_type : uint32_t
	{
		VIF0 = 0,
		VIF1,
		IPU_OUT,
		IPU_IN,
		SIF0,
		SIF1,
		SIF2,
		SPR_OUT,
		SPR_IN
	};

	DECLARE_READ32_MEMBER(read);
	DECLARE_WRITE32_MEMBER(write);
	DECLARE_READ32_MEMBER(channel_r);
	DECLARE_WRITE32_MEMBER(channel_w);

	DECLARE_READ32_MEMBER(disable_mask_r);
	DECLARE_WRITE32_MEMBER(disable_mask_w);

protected:
    virtual void device_start() override;
    virtual void device_reset() override;
	virtual void execute_run() override;

	class channel_t
	{
		friend class ps2_dmac_device;

	public:
		channel_t()
			: m_chcr(0), m_mode(0), m_enabled(false), m_end_tag(false), m_addr(0), m_qwc(0)
		{
		}

		void set_chcr(uint32_t data);
		uint32_t chcr() const { return m_chcr; }
		uint32_t mode() const { return m_mode; }
		bool enabled() const { return m_enabled; }
		bool end_tag() const { return m_end_tag; }

		uint32_t addr() const { return m_addr; }
		void set_addr(uint32_t addr) { m_addr = addr; }

		uint32_t quadword_count() const { return m_qwc; }
		void set_quadword_count(uint32_t qwc) { m_qwc = qwc; }

	protected:
		uint32_t m_chcr;
		uint32_t m_mode;
		bool m_enabled;
		bool m_end_tag;

		uint32_t m_addr;
		uint32_t m_qwc;
	};

	void transfer_sif0();
	void transfer_finish(uint32_t chan);
	void follow_tag(uint32_t channel);
	void update_interrupts();

	required_device<cpu_device> m_ee;
	required_shared_ptr<uint64_t> m_ram;
	required_device<ps2_sif_device> m_sif;

	int m_icount;

	uint32_t m_ctrl;
	uint32_t m_mem_drain;
	bool m_enabled;

	uint32_t m_disable_mask;

	uint32_t m_istat;
	uint32_t m_imask;

	uint32_t m_last_serviced;
	channel_t m_channels[10];
};

DECLARE_DEVICE_TYPE(SONYPS2_DMAC, ps2_dmac_device)

#endif // MAME_MACHINE_PS2DMAC_H