/*
 * Author: Andrew Powell
 * Purpose: Small example application, demonstrating how the ADI I2C core
 * and the SSM2603 audio codec of the Zybo board can be utilized with a
 * c driver.
 */

#include "xparameters.h"
#include "xil_assert.h"
#include "xil_io.h"
#include "sleep.h"
#include "xiicps.h"
#include "audio_cppwrap.h"

#define IIC_DEVICE_ID		( XPAR_XIICPS_0_DEVICE_ID )
#define IIC_SCLK_RATE		( 100000 )
#define ADI_I2S_PHY_ADDR	( XPAR_AXI_I2S_ADI_0_S_AXI_BASEADDR )
#define ADI_I2S_VOL_SHIFT   ( 2 )

/* Define the low-level behavior for the audio driver. */
class audio_driver : public audio_cppwrap
{
public:
private:
	void mem_access( audio_dir dir, uint32_t phy_addr, uint32_t* data, void* param );
	void i2c_trans( audio_dir dir, uint8_t slave_addr, uint8_t* data, size_t len, void* param );
	void i2c_delay( unsigned int ms, void* param );
};

/* This inline function is needed to convert the unsigned values to proper signed values. */
inline int32_t convert_sample( uint32_t sample ) { return static_cast<int32_t>( sample << 8 ) >> 8; }

/* Object related to this example application. */
XIicPs i2c_obj;
audio_driver audio_obj;

int main()
{
	/* First step, configure the I2C driver! */
	{
		int Status;
		XIicPs_Config *Config;

		Config = XIicPs_LookupConfig( IIC_DEVICE_ID );
		Xil_AssertNonvoid( Config!=NULL );
		Status = XIicPs_CfgInitialize( &i2c_obj, Config, Config->BaseAddress );
		Xil_AssertNonvoid( Status==XST_SUCCESS );
		XIicPs_SetSClk( &i2c_obj, IIC_SCLK_RATE );
	}

	/* Start the audio driver and reset TX and RX buffers of
	the ADI core. */
	audio_obj.start( ADI_I2S_PHY_ADDR, NULL );
	audio_obj.write_mem_reset_txrx();

	/* This application will run indefinitely. */
	while ( true )
	{
		/* Grab left and right samples from the ADI core's RX buffer. */
		uint32_t* recv_send = audio_obj.read_mem_sample_lr();

		/* Ensure the bits are reinterpreted as signed values. */
		int32_t samples[] =
		{
				convert_sample( recv_send[ 0 ] ),
				convert_sample( recv_send[ 1 ] )
		};

		/* Any sort of manipulation to the samples should be
		done with the following block. */
		{
			/* Increase the volume so that the channels can be heard. */
			samples[ 0 ] <<= ADI_I2S_VOL_SHIFT;
			samples[ 1 ] <<= ADI_I2S_VOL_SHIFT;
		}

		/* Store new samples back into send/receive buffer. */
		recv_send[ 0 ] = static_cast<uint32_t>( samples[ 0 ] );
		recv_send[ 1 ] = static_cast<uint32_t>( samples[ 1 ] );

		/* Output samples to ADI core's TX buffer. */
		audio_obj.write_mem_sample_lr( recv_send );
	}
	return 0;
}

void audio_driver::mem_access( audio_dir dir, uint32_t phy_addr, uint32_t* data, void* param )
{
	( void ) param;
	switch ( dir )
	{
	case audio_dir_WRITE:
		{
			Xil_Out32( phy_addr, *data );
		}
		break;
	case audio_dir_READ:
		{
			*data = Xil_In32( phy_addr );
		}
		break;
	}
}

void audio_driver::i2c_trans( audio_dir dir, uint8_t slave_addr, uint8_t* data, size_t len, void* param )
{
	( void ) param;
	int Status;
	switch ( dir )
	{
	case audio_dir_WRITE:
		{
			Status = XIicPs_MasterSendPolled( &i2c_obj, data, len, slave_addr );
		}
		break;
	case audio_dir_READ:
		{
			Status = XIicPs_MasterRecvPolled( &i2c_obj, data, len, slave_addr );
		}
		break;
	}
	Xil_AssertVoid( Status==XST_SUCCESS );
}

void audio_driver::i2c_delay( unsigned int ms, void* param )
{
	( void ) param;
	usleep( ms*1000 );
}


