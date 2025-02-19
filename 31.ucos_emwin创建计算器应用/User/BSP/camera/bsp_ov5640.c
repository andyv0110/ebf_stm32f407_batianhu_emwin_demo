/**
  ******************************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   OV5640摄像头驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
//#include "./systick/bsp_SysTick.h"
#include "./camera/bsp_ov5640.h"
#include "./camera/ov5640_AF.h"
/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */
	
	
	
//摄像头初始化配置
//注意：使用这种方式初始化结构体，要在c/c++选项中选择 C99 mode
OV5640_MODE_PARAM cam_mode =
{
	
/*以下包含几组摄像头配置，可自行测试，保留一组，把其余配置注释掉即可*/
/************配置1***854*480******横屏显示*****************************/
	
	.frame_rate = FRAME_RATE_15FPS,	
	
	//ISP窗口
	.cam_isp_sx = 0,
	.cam_isp_sy = 0,	
	
	.cam_isp_width = 1920,
	.cam_isp_height = 1080,
	
	//输出窗口
	.scaling = 1,      //使能自动缩放
	.cam_out_sx = 16,	//使能自动缩放后，一般配置成16即可
	.cam_out_sy = 4,	  //使能自动缩放后，一般配置成4即可
	.cam_out_width = 854,
	.cam_out_height = 480,
	
	//LCD位置
	.lcd_sx = 0,
	.lcd_sy = 0,
	.lcd_scan = 5, //LCD扫描模式，本横屏配置可用1、3、5、7模式
	
	//以下可根据自己的需要调整，参数范围见结构体类型定义	
	.light_mode = 0,//自动光照模式
	.saturation = 0,	
	.brightness = 0,
	.contrast = 0,
	.effect = 0,		//正常模式
	.exposure = 0,		

	.auto_focus = 1,
	
/**********配置2*****240*320****竖屏显示****************************/	
//	.frame_rate = FRAME_RATE_30FPS,	
//	
//	//ISP窗口
//	.cam_isp_sx = 0,
//	.cam_isp_sy = 0,	
//	
//	.cam_isp_width = 1920,
//	.cam_isp_height = 1080,
//	
//	//输出窗口
//	.scaling = 1,      //使能自动缩放
//	.cam_out_sx = 16,	//使能自动缩放后，一般配置成16即可
//	.cam_out_sy = 4,	  //使能自动缩放后，一般配置成4即可
//	.cam_out_width = 240,
//	.cam_out_height = 320,
//	
//	//LCD位置
//	.lcd_sx = 120,
//	.lcd_sy = 267,
//	.lcd_scan = 6, //LCD扫描模式，
//	
//	//以下可根据自己的需要调整，参数范围见结构体类型定义	
//	.light_mode = 0,//自动光照模式
//	.saturation = 0,	
//	.brightness = 0,
//	.contrast = 0,
//	.effect = 0,		//正常模式
//	.exposure = 0,		

//	.auto_focus = 1,//自动对焦
	
	/*******配置3********640*480****小分辨率****************************/	
//  .frame_rate = FRAME_RATE_30FPS,	
//	
//	//ISP窗口
//	.cam_isp_sx = 0,
//	.cam_isp_sy = 0,	
//	
//	.cam_isp_width = 1920,
//	.cam_isp_height = 1080,
//	
//	//输出窗口
//	.scaling = 0,      //使能自动缩放
//	.cam_out_sx = 16,	//使能自动缩放后，一般配置成16即可
//	.cam_out_sy = 4,	  //使能自动缩放后，一般配置成4即可
//	.cam_out_width = 640,
//	.cam_out_height = 480,
//	
//	//LCD位置
//	.lcd_sx = 100,
//	.lcd_sy = 0,
//	.lcd_scan = 5, //LCD扫描模式，
//	
//	//以下可根据自己的需要调整，参数范围见结构体类型定义	
//	.light_mode = 0,//自动光照模式
//	.saturation = 0,	
//	.brightness = 0,
//	.contrast = 0,
//	.effect = 0,		//正常模式
//	.exposure = 0,		

//	.auto_focus = 1,//自动对焦

};

/** @addtogroup DCMI_Camera
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  TIMEOUT  2



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

unsigned short RGB565_Init[][2]=
{
    //15fps VGA RGB565 output
    // 24MHz input clock, 24MHz PCLK
    0x3103, 0x11, // system clock from pad, bit[1]
    0x3008, 0x82, // software reset, bit[7]
    // delay 5ms
    0x3008, 0x42, // software power down, bit[6]
    0x3103, 0x03, // system clock from PLL, bit[1]
    0x3017, 0xff, // FREX, Vsync, HREF, PCLK, D[9:6] output enable
    0x3018, 0xff, // D[5:0], GPIO[1:0] output enable
    0x3034, 0x1a, // MIPI 10-bit
    0x3037, 0x13, // PLL root divider, bit[4], PLL pre-divider, bit[3:0]
    0x3108, 0x01, // PCLK root divider, bit[5:4], SCLK2x root divider, bit[3:2]
    // SCLK root divider, bit[1:0]
    0x3630, 0x36,
    0x3631, 0x0e,
    0x3632, 0xe2,
    0x3633, 0x12,
    0x3621, 0xe0,
    0x3704, 0xa0,
    0x3703, 0x5a,
    0x3715, 0x78,
    0x3717, 0x01,
    0x370b, 0x60,
    0x3705, 0x1a,
    0x3905, 0x02,
    0x3906, 0x10,
    0x3901, 0x0a,
    0x3731, 0x12,
    0x3600, 0x08, // VCM control
    0x3601, 0x33, // VCM control
    0x302d, 0x60, // system control
    0x3620, 0x52,
    0x371b, 0x20,
    0x471c, 0x50,
    0x3a13, 0x43, // pre-gain = 1.047x
    0x3a18, 0x00, // gain ceiling
    0x3a19, 0xf8, // gain ceiling = 15.5x
    0x3635, 0x13,
    0x3636, 0x03,
    0x3634, 0x40,
    0x3622, 0x01,
    // 50/60Hz detection 50/60Hz 灯光条纹过滤
    0x3c01, 0x34, // Band auto, bit[7]
    0x3c04, 0x28, // threshold low sum
    0x3c05, 0x98, // threshold high sum
    0x3c06, 0x00, // light meter 1 threshold[15:8]
    0x3c07, 0x08, // light meter 1 threshold[7:0]
    0x3c08, 0x00, // light meter 2 threshold[15:8]
    0x3c09, 0x1c, // light meter 2 threshold[7:0]
    0x3c0a, 0x9c, // sample number[15:8]
    0x3c0b, 0x40, // sample number[7:0]
    0x3810, 0x00, // Timing Hoffset[11:8]
    0x3811, 0x10, // Timing Hoffset[7:0]
    0x3812, 0x00, // Timing Voffset[10:8]
    0x3708, 0x64,
    0x4001, 0x02, // BLC start from line 2
    0x4005, 0x1a, // BLC always update
    0x3000, 0x00, // enable blocks
    0x3004, 0xff, // enable clocks
    0x300e, 0x58, // MIPI power down, DVP enable
    0x302e, 0x00,

    0x4300, 0x6f, // RGB565
    0x501f, 0x01, // RGB565

    0x440e, 0x00,
    0x5000, 0xa7, // Lenc on, raw gamma on, BPC on, WPC on, CIP on
    // AEC target 自动曝光控制
    0x3a0f, 0x30, // stable range in high
    0x3a10, 0x28, // stable range in low
    0x3a1b, 0x30, // stable range out high
    0x3a1e, 0x26, // stable range out low
    0x3a11, 0x60, // fast zone high
    0x3a1f, 0x14, // fast zone low
    // Lens correction for ? 镜头补偿
    0x5800, 0x23,
    0x5801, 0x14,
    0x5802, 0x0f,
    0x5803, 0x0f,
    0x5804, 0x12,
    0x5805, 0x26,
    0x5806, 0x0c,
    0x5807, 0x08,
    0x5808, 0x05,
    0x5809, 0x05,
    0x580a, 0x08,
    0x580b, 0x0d,
    0x580c, 0x08,
    0x580d, 0x03,
    0x580e, 0x00,
    0x580f, 0x00,
    0x5810, 0x03,
    0x5811, 0x09,
    0x5812, 0x07,
    0x5813, 0x03,
    0x5814, 0x00,
    0x5815, 0x01,
    0x5816, 0x03,
    0x5817, 0x08,
    0x5818, 0x0d,
    0x5819, 0x08,
    0x581a, 0x05,
    0x581b, 0x06,
    0x581c, 0x08,
    0x581d, 0x0e,
    0x581e, 0x29,
    0x581f, 0x17,
    0x5820, 0x11,
    0x5821, 0x11,
    0x5822, 0x15,
    0x5823, 0x28,
    0x5824, 0x46,
    0x5825, 0x26,
    0x5826, 0x08,
    0x5827, 0x26,
    0x5828, 0x64,
    0x5829, 0x26,
    0x582a, 0x24,
    0x582b, 0x22,
    0x582c, 0x24,
    0x582d, 0x24,
    0x582e, 0x06,
    0x582f, 0x22,
    0x5830, 0x40,
    0x5831, 0x42,
    0x5832, 0x24,
    0x5833, 0x26,
    0x5834, 0x24,
    0x5835, 0x22,
    0x5836, 0x22,
    0x5837, 0x26,
    0x5838, 0x44,
    0x5839, 0x24,
    0x583a, 0x26,
    0x583b, 0x28,
    0x583c, 0x42,
    0x583d, 0xce, // lenc BR offset
    // AWB 自动白平衡
    0x5180, 0xff, // AWB B block
    0x5181, 0xf2, // AWB control
    0x5182, 0x00, // [7:4] max local counter, [3:0] max fast counter
    0x5183, 0x14, // AWB advanced
    0x5184, 0x25,
    0x5185, 0x24,
    0x5186, 0x09,
    0x5187, 0x09,
    0x5188, 0x09,
    0x5189, 0x75,
    0x518a, 0x54,
    0x518b, 0xe0,
    0x518c, 0xb2,
    0x518d, 0x42,
    0x518e, 0x3d,
    0x518f, 0x56,
    0x5190, 0x46,
    0x5191, 0xf8, // AWB top limit
    0x5192, 0x04, // AWB bottom limit
    0x5193, 0x70, // red limit
    0x5194, 0xf0, // green limit
    0x5195, 0xf0, // blue limit
    0x5196, 0x03, // AWB control
    0x5197, 0x01, // local limit
    0x5198, 0x04,
    0x5199, 0x12,
    0x519a, 0x04,
    0x519b, 0x00,
    0x519c, 0x06,
    0x519d, 0x82,
    0x519e, 0x38, // AWB control
    // Gamma 伽玛曲线
    0x5480, 0x01, // Gamma bias plus on, bit[0]
    0x5481, 0x08,
    0x5482, 0x14,
    0x5483, 0x28,
    0x5484, 0x51,
    0x5485, 0x65,
    0x5486, 0x71,
    0x5487, 0x7d,
    0x5488, 0x87,
    0x5489, 0x91,
    0x548a, 0x9a,
    0x548b, 0xaa,
    0x548c, 0xb8,
    0x548d, 0xcd,
    0x548e, 0xdd,
    0x548f, 0xea,
    0x5490, 0x1d,
    // color matrix 色彩矩阵
    0x5381, 0x1e, // CMX1 for Y
    0x5382, 0x5b, // CMX2 for Y
    0x5383, 0x08, // CMX3 for Y
    0x5384, 0x0a, // CMX4 for U
    0x5385, 0x7e, // CMX5 for U
    0x5386, 0x88, // CMX6 for U
    0x5387, 0x7c, // CMX7 for V
    0x5388, 0x6c, // CMX8 for V
    0x5389, 0x10, // CMX9 for V
    0x538a, 0x01, // sign[9]
    0x538b, 0x98, // sign[8:1]
    // UV adjust UV 色彩饱和度调整
    0x5580, 0x06, // saturation on, bit[1]
    0x5583, 0x40,
    0x5584, 0x10,
    0x5589, 0x10,
    0x558a, 0x00,
    0x558b, 0xf8,
    0x501d, 0x40, // enable manual offset of contrast
    // CIP 锐化和降噪
    0x5300, 0x08, // CIP sharpen MT threshold 1
    0x5301, 0x30, // CIP sharpen MT threshold 2
    0x5302, 0x10, // CIP sharpen MT offset 1
    0x5303, 0x00, // CIP sharpen MT offset 2
    0x5304, 0x08, // CIP DNS threshold 1
    0x5305, 0x30, // CIP DNS threshold 2
    0x5306, 0x08, // CIP DNS offset 1
    0x5307, 0x16, // CIP DNS offset 2
    0x5309, 0x08, // CIP sharpen TH threshold 1
    0x530a, 0x30, // CIP sharpen TH threshold 2
    0x530b, 0x04, // CIP sharpen TH offset 1
    0x530c, 0x06, // CIP sharpen TH offset 2
    0x5025, 0x00,
    0x3008, 0x02, // wake up from standby, bit[6]
   
//    0x503d, 0x80,//测试彩条
//    0x4741, 0x00,
};


unsigned short RGB565_WVGA[][2]=
{
    // 30fps, night mode 5fps
    // input clock 24Mhz, PCLK 45.6Mhz
		//30fps
    0x3035, 0x21, // PLL
    0x3036, 0x72, // PLL
	
		//15fps
//	  0x3035, 0x41, // PLL
//    0x3036, 0x72, // PLL
	
    0x3c07, 0x08, // light meter 1 threshold[7:0]
    0x3820, 0x42, // flip
//    0x3821, 0x07, // mirror
    0x3814, 0x31, // timing X inc
    0x3815, 0x31, // timing Y inc
    0x3800, 0x00, // HS
    0x3801, 0x00, // HS
    0x3802, 0x00, // VS
    0x3803, 0xbe, // VS
    0x3804, 0x0a, // HW (HE)
    0x3805, 0x3f, // HW (HE)
    0x3806, 0x06, // VH (VE)
    0x3807, 0xe4, // VH (VE)
    0x3808, 0x03, // DVPHO
    0x3809, 0x56, // DVPHO
    0x380a, 0x01, // DVPVO
    0x380b, 0xe0, // DVPVO
    0x380c, 0x07, // HTS
    0x380d, 0x69, // HTS
    0x380e, 0x03, // VTS
    0x380f, 0x21, // VTS
    0x3813, 0x06, // timing V offset
    0x3618, 0x00,
    0x3612, 0x29,
    0x3709, 0x52,
    0x370c, 0x03,
    0x3a02, 0x09, // 60Hz max exposure, night mode 5fps
    0x3a03, 0x63, // 60Hz max exposure
    // banding filters are calculated automatically in camera driver
    //0x3a08, 0x00, // B50 step
    //0x3a09, 0x78, // B50 step
    //0x3a0a, 0x00, // B60 step
    //0x3a0b, 0x64, // B60 step
    //0x3a0e, 0x06, // 50Hz max band
    //0x3a0d, 0x08, // 60Hz max band
    0x3a14, 0x09, // 50Hz max exposure, night mode 5fps
    0x3a15, 0x63, // 50Hz max exposure
    0x4004, 0x02, // BLC line number
    0x3002, 0x1c, // reset JFIFO, SFIFO, JPG
    0x3006, 0xc3, // disable clock of JPEG2x, JPEG
    0x4713, 0x03, // JPEG mode 3
    0x4407, 0x04, // Quantization sacle
    0x460b, 0x35,
    0x460c, 0x22,
    0x4837, 0x22, // MIPI global timing
    0x3824, 0x02, // PCLK manual divider
    0x5001, 0xa3, // SDE on, CMX on, AWB on
    0x3503, 0x00, // AEC/AGC on//	  /* Initialize OV5640 */
    
    
//    0x3016, 0x01,
//    0x301C, 0x00,
//    0x3016, 0x01,
//    0x301C, 0x01,
//    0x3019, 0x01,

//    0x503d, 0x80,//测试彩条
//    0x4741, 0x00,
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  初始化控制摄像头使用的GPIO(I2C/DCMI)
  * @param  None
  * @retval None
  */
void OV5640_HW_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  I2C_InitTypeDef  I2C_InitStruct;

	  /***DCMI引脚配置***/
	  /* 使能DCMI时钟 */
	  RCC_AHB1PeriphClockCmd(DCMI_PWDN_GPIO_CLK|DCMI_RST_GPIO_CLK|DCMI_VSYNC_GPIO_CLK | DCMI_HSYNC_GPIO_CLK | DCMI_PIXCLK_GPIO_CLK|
			  DCMI_D0_GPIO_CLK| DCMI_D1_GPIO_CLK| DCMI_D2_GPIO_CLK| DCMI_D3_GPIO_CLK|
			  DCMI_D4_GPIO_CLK| DCMI_D5_GPIO_CLK| DCMI_D6_GPIO_CLK| DCMI_D7_GPIO_CLK, ENABLE);

	  /*控制/同步信号线*/
	  GPIO_InitStructure.GPIO_Pin = DCMI_VSYNC_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	  GPIO_Init(DCMI_VSYNC_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(DCMI_VSYNC_GPIO_PORT, DCMI_VSYNC_PINSOURCE, DCMI_VSYNC_AF);

	  GPIO_InitStructure.GPIO_Pin = DCMI_HSYNC_GPIO_PIN ;
	  GPIO_Init(DCMI_HSYNC_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(DCMI_HSYNC_GPIO_PORT, DCMI_HSYNC_PINSOURCE, DCMI_HSYNC_AF);

	  GPIO_InitStructure.GPIO_Pin = DCMI_PIXCLK_GPIO_PIN ;
	  GPIO_Init(DCMI_PIXCLK_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(DCMI_PIXCLK_GPIO_PORT, DCMI_PIXCLK_PINSOURCE, DCMI_PIXCLK_AF);
    
		/*数据信号*/
	  GPIO_InitStructure.GPIO_Pin = DCMI_D0_GPIO_PIN ;
	  GPIO_Init(DCMI_D0_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(DCMI_D0_GPIO_PORT, DCMI_D0_PINSOURCE, DCMI_D0_AF);

	  GPIO_InitStructure.GPIO_Pin = DCMI_D1_GPIO_PIN ;
	  GPIO_Init(DCMI_D1_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(DCMI_D1_GPIO_PORT, DCMI_D1_PINSOURCE, DCMI_D1_AF);

	  GPIO_InitStructure.GPIO_Pin = DCMI_D2_GPIO_PIN ;
	  GPIO_Init(DCMI_D2_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(DCMI_D2_GPIO_PORT, DCMI_D2_PINSOURCE, DCMI_D2_AF);

	  GPIO_InitStructure.GPIO_Pin = DCMI_D3_GPIO_PIN ;
	  GPIO_Init(DCMI_D3_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(DCMI_D3_GPIO_PORT, DCMI_D3_PINSOURCE, DCMI_D3_AF);

	  GPIO_InitStructure.GPIO_Pin = DCMI_D4_GPIO_PIN ;
	  GPIO_Init(DCMI_D4_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(DCMI_D4_GPIO_PORT, DCMI_D4_PINSOURCE, DCMI_D4_AF);

	  GPIO_InitStructure.GPIO_Pin = DCMI_D5_GPIO_PIN ;
	  GPIO_Init(DCMI_D5_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(DCMI_D5_GPIO_PORT, DCMI_D5_PINSOURCE, DCMI_D5_AF);

	  GPIO_InitStructure.GPIO_Pin = DCMI_D6_GPIO_PIN ;
	  GPIO_Init(DCMI_D6_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(DCMI_D6_GPIO_PORT, DCMI_D6_PINSOURCE, DCMI_D6_AF);

	  GPIO_InitStructure.GPIO_Pin = DCMI_D7_GPIO_PIN ;
	  GPIO_Init(DCMI_D7_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(DCMI_D7_GPIO_PORT, DCMI_D7_PINSOURCE, DCMI_D7_AF);



      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_InitStructure.GPIO_Pin = DCMI_PWDN_GPIO_PIN ;
      GPIO_Init(DCMI_PWDN_GPIO_PORT, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin = DCMI_RST_GPIO_PIN ;
      GPIO_Init(DCMI_RST_GPIO_PORT, &GPIO_InitStructure);
      /*PWDN引脚，高电平关闭电源，低电平供电*/
     
      GPIO_ResetBits(DCMI_RST_GPIO_PORT,DCMI_RST_GPIO_PIN);
      GPIO_SetBits(DCMI_PWDN_GPIO_PORT,DCMI_PWDN_GPIO_PIN);
       
      Delay(10);
      
      GPIO_ResetBits(DCMI_PWDN_GPIO_PORT,DCMI_PWDN_GPIO_PIN);
      
      Delay(10);
      
      GPIO_SetBits(DCMI_RST_GPIO_PORT,DCMI_RST_GPIO_PIN);
      
	  /****** 配置I2C，使用I2C与摄像头的SCCB接口通讯*****/
	 /* 使能I2C时钟 */
	  RCC_APB1PeriphClockCmd(CAMERA_I2C_CLK, ENABLE);
	  /* 使能I2C使用的GPIO时钟 */
	  RCC_AHB1PeriphClockCmd(CAMERA_I2C_SCL_GPIO_CLK|CAMERA_I2C_SDA_GPIO_CLK, ENABLE);
	  /* 配置引脚源 */
	  GPIO_PinAFConfig(CAMERA_I2C_SCL_GPIO_PORT, CAMERA_I2C_SCL_SOURCE, CAMERA_I2C_SCL_AF);
	  GPIO_PinAFConfig(CAMERA_I2C_SDA_GPIO_PORT, CAMERA_I2C_SDA_SOURCE, CAMERA_I2C_SDA_AF);

	  /* 初始化GPIO */
	  GPIO_InitStructure.GPIO_Pin = CAMERA_I2C_SCL_PIN ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(CAMERA_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(CAMERA_I2C_SCL_GPIO_PORT, CAMERA_I2C_SCL_SOURCE, CAMERA_I2C_SCL_AF);

	  GPIO_InitStructure.GPIO_Pin = CAMERA_I2C_SDA_PIN ;
	  GPIO_Init(CAMERA_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

	  /*初始化I2C模式 */
	  I2C_DeInit(CAMERA_I2C); 

	  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	  I2C_InitStruct.I2C_OwnAddress1 = 0xFE;
	  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	  I2C_InitStruct.I2C_ClockSpeed = 400000;

	  /* 写入配置 */
	  I2C_Init(CAMERA_I2C, &I2C_InitStruct);
		
		 /* 使能I2C */
	  I2C_Cmd(CAMERA_I2C, ENABLE);
      
      Delay(50);
}

/**
  * @brief  Resets the OV5640 camera.
  * @param  None
  * @retval None
  */
void OV5640_Reset(void)
{
	/*OV5640软件复位*/
  OV5640_WriteReg(0x3008, 0x80);
}

/**
  * @brief  读取摄像头的ID.
  * @param  OV5640ID: 存储ID的结构体
  * @retval None
  */
void OV5640_ReadID(OV5640_IDTypeDef *OV5640ID)
{

	/*读取寄存芯片ID*/
  OV5640ID->PIDH = OV5640_ReadReg(OV5640_SENSOR_PIDH);
  OV5640ID->PIDL = OV5640_ReadReg(OV5640_SENSOR_PIDL);
}



/**
  * @brief  配置 DCMI/DMA 以捕获摄像头数据
  * @param  None
  * @retval None
  */
void OV5640_Init(void) 
{
  DCMI_InitTypeDef DCMI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /*** 配置DCMI接口 ***/
  /* 使能DCMI时钟 */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);

  /* DCMI 配置*/ 
  DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
  DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
  DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;
  DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
  DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;
  DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
  DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
  DCMI_Init(&DCMI_InitStructure); 	
	
	//配置DMA传输，直接配置循环传输即可
  OV5640_DMA_Config((uint32_t)FSMC_LCD_ADDRESS,1); 	

 	/* 配置帧中断，接收到帧同步信号就进入中断 */
	//中断服务函数在bsp_ov2640.c文件末尾中 
	NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn ;	//帧中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DCMI_ITConfig (DCMI_IT_FRAME,ENABLE);	

}

/**
  * @brief  开启或关闭DCMI采集
	* @param  ENABLE或DISABLE
  */
void OV5640_Capture_Control(FunctionalState state)
{
		DMA_Cmd(DMA2_Stream1, state);//DMA2,Stream1
  	DCMI_Cmd(state); 						//DCMI采集数据
		DCMI_CaptureCmd(state);//DCMI捕获
}

/**
  * @brief  关闭摄像头
  */
void OV5640_Stop(void)
{  
	OV5640_Capture_Control(DISABLE);
  GPIO_ResetBits(DCMI_RST_GPIO_PORT,DCMI_RST_GPIO_PIN);
  Delay(10); 
  GPIO_SetBits(DCMI_PWDN_GPIO_PORT,DCMI_PWDN_GPIO_PIN);
}
/**
  * @brief  配置 DCMI/DMA 以捕获摄像头数据
	* @param  DMA_Memory0BaseAddr:本次传输的目的首地址
  * @param DMA_BufferSize：本次传输的数据量(单位为字,即4字节)
  */
void OV5640_DMA_Config(uint32_t DMA_Memory0BaseAddr,uint16_t DMA_BufferSize)
{

  DMA_InitTypeDef  DMA_InitStructure;
  
  /* 配置DMA从DCMI中获取数据*/
  /* 使能DMA*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  
  DMA_Cmd(DMA2_Stream1,DISABLE);
  while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}	

  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	//DCMI数据寄存器地址
  DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr; //DMA传输的目的地址(传入的参数)	
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize =DMA_BufferSize; 						//传输的数据大小(传入的参数)
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;			//寄存器地址自增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//循环模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC8;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  /*DMA中断配置 */
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
  
  DMA_Cmd(DMA2_Stream1,ENABLE);
	while(DMA_GetCmdStatus(DMA2_Stream1) != ENABLE){}
}

/**
  * @brief  设置帧率
  * @param  可使用宏：FRAME_RATE_30FPS，FRAME_RATE_15FPS
  * @retval 无
  */
void OV5640_FrameRate_Set(uint8_t frame_rate)
{ 
	if(frame_rate == FRAME_RATE_30FPS)
	{
		OV5640_WriteReg(0x3035,0x21);		//X_ADDR_ST高字节
    OV5640_WriteReg(0x3036,0x72);//X_ADDR_ST低字节	

	}
	else if(frame_rate == FRAME_RATE_15FPS)
	{
		OV5640_WriteReg(0x3035,0x41);		//X_ADDR_ST高字节
    OV5640_WriteReg(0x3036,0x72);//X_ADDR_ST低字节	

	}
	
}

/**
  * @brief  设置ISP图像大小，即ISP图像在传感器中的窗口
  *         较小的ISP图像可以提高图像采集频率(还要设置时钟)
  * @param  x_st,y_st：起始位置偏移
	* @param  width，height: 宽度，高度
  * @note   x_st+width <= 2592,y_st+height <= 1944
  * @retval 无
  */
void OV5640_ISPSize_Set(uint16_t x_st,uint16_t y_st,uint16_t width,uint16_t height)
{ 
//    OV5640_WriteReg(0X3212,0X03);  	
	
	  OV5640_WriteReg(0x3800,x_st>>8);		//X_ADDR_ST高字节
    OV5640_WriteReg(0x3801,x_st&0xff);//X_ADDR_ST低字节	
    OV5640_WriteReg(0x3802,y_st>>8);		//Y_ADDR_ST高字节
    OV5640_WriteReg(0x3803,y_st&0xff);//Y_ADDR_ST低字节
	
		OV5640_WriteReg(0x3804,(x_st+width)>>8);		//X_ADDR_END高字节
    OV5640_WriteReg(0x3805,(x_st+width)&0xff);	//X_ADDR_END低字节	
    OV5640_WriteReg(0x3806,(y_st+height)>>8);		//Y_ADDR_END高字节
    OV5640_WriteReg(0x3807,(y_st+height)&0xff);//Y_ADDR_END低字节

//    OV5640_WriteReg(0X3212,0X13);		
//    OV5640_WriteReg(0X3212,0Xa3);		
}





/**
  * @brief  设置图像输出像大小，位置，以及是否使用自动缩放功能
  * @param  scaling:0,关闭自动缩放功能，1，开启自动缩放功能
  * @param  x_off,y_off:-关闭自动缩放功能时，这两个值为输出窗口在ISP图像中的偏移。
												 ！！不使用自动缩放功能时，非常容易出错。

                         ！！使用15fps相对不容易出错，但还是推荐使用自动缩放功能。

                        -开启自动缩放功能时，这两个值设置为 16,4 一般能正常使用，
                        也可根据输出窗口的宽高比例来调整，比较复杂，
                        请参考《OV5640 自动对焦摄像模组应用指南》第46页
  * @param  width,height:图像宽度和图像高度
  * @retval 无
  */
void OV5640_OutSize_Set(uint8_t scaling,uint16_t x_off,uint16_t y_off,uint16_t width,uint16_t height)
{ 
		uint8_t reg_temp;
	
		//读取原寄存器内容
		reg_temp = OV5640_ReadReg(0x5001);
	
		//不使用自动缩放功能
		if(scaling == 0 )
		{
			OV5640_WriteReg(0x5001,reg_temp &~(1<<5) );	//scaling off		
		}
		else
		{
			OV5640_WriteReg(0x5001,reg_temp|(1<<5) );	//scaling on		
		}	
	
		OV5640_WriteReg(0x3810,x_off>>8);	//X offset高字节
		OV5640_WriteReg(0x3811,x_off&0xff);//X offset低字节	
		OV5640_WriteReg(0x3812,y_off>>8);	//Y offset高字节
		OV5640_WriteReg(0x3813,y_off&0xff);//Y offset低字节
		
	
    OV5640_WriteReg(0X3212,0X03);  	
	
    OV5640_WriteReg(0x3808,width>>8);	//输出宽度高字节
    OV5640_WriteReg(0x3809,width&0xff);//输出宽度低字节  
    OV5640_WriteReg(0x380a,height>>8);//输出高度高字节
    OV5640_WriteReg(0x380b,height&0xff);//输出高度低字节
	
    OV5640_WriteReg(0X3212,0X13);		
    OV5640_WriteReg(0X3212,0Xa3);		
}





unsigned short sensor_reg[(sizeof(RGB565_Init)/4)];
/**
  * @brief  配置 OV5640为 BMP 模式.
  * @retval None
  */
void OV5640_RGB565_Default_Config(void)
{
  uint32_t i;
  
	/*摄像头复位*/
  OV5640_Reset();
	
	Delay(50);

  /* 写入寄存器配置RGB565模式 */
	for(i=0; i<(sizeof(RGB565_Init)/4); i++)
  {
    OV5640_WriteReg(RGB565_Init[i][0], RGB565_Init[i][1]);
		
		Delay(5);

    sensor_reg[i] = OV5640_ReadReg(RGB565_Init[i][0]);
		
		CAMERA_DEBUG("sensor_reg[0x%x]:%x-%x\n",RGB565_Init[i][0],RGB565_Init[i][1],sensor_reg[i]);

  }

  Delay(100);
       
	//以默认模式初始化
	for(i=0; i<(sizeof(RGB565_WVGA)/2); i++)
	{
		OV5640_WriteReg(RGB565_WVGA[i][0], RGB565_WVGA[i][1]);
	}
   
}


//环境光模式参数表
const static uint8_t OV5640_LightMode_reg[][7]=
{ 
	0x04,0X00,0X04,0X00,0X04,0X00,0X00,//Auto,自动 
	0x06,0X1C,0X04,0X00,0X04,0XF3,0X01,//Sunny,日光
	0x05,0X48,0X04,0X00,0X07,0XCF,0X01,//Office,办公室
	0x06,0X48,0X04,0X00,0X04,0XD3,0X01,//Cloudy,阴天 
	0x04,0X10,0X04,0X00,0X08,0X40,0X01,//Home,室内
}; 
/**
  * @brief  配置光线模式
  * @param  参数用于选择光线模式
  *         0x00 Auto     自动
  *         0x01 Sunny    日光
  *         0x02 Office 	办公室
  *         0x03 Cloudy		阴天 
  *         0x04 Home			室内

  * @retval None
  */
void OV5640_LightMode(uint8_t mode)
{
  OV5640_WriteReg(0x3212, 0x03); // start group 3
	
	OV5640_WriteReg(0x3406, OV5640_LightMode_reg[mode][0]);
	OV5640_WriteReg(0x3400, OV5640_LightMode_reg[mode][1]);
	OV5640_WriteReg(0x3401, OV5640_LightMode_reg[mode][2]);
	OV5640_WriteReg(0x3402, OV5640_LightMode_reg[mode][3]);
	OV5640_WriteReg(0x3403, OV5640_LightMode_reg[mode][4]);
	OV5640_WriteReg(0x3404, OV5640_LightMode_reg[mode][5]);
	OV5640_WriteReg(0x3405, OV5640_LightMode_reg[mode][6]);
	
	OV5640_WriteReg(0x3212, 0x13); // end group 3
	OV5640_WriteReg(0x3212, 0xa3); // lanuch group 3
}


//特效设置参数表
const static uint8_t OV5640_Effect_reg[][4]=
{ 
	0X06,0x40,0X10,0X08,//正常 
	0X1E,0xA0,0X40,0X08,//冷色
	0X1E,0x80,0XC0,0X08,//暖色
	0X1E,0x80,0X80,0X08,//黑白
	0X1E,0x40,0XA0,0X08,//泛黄 
	0X40,0x40,0X10,0X08,//反色
	0X1E,0x60,0X60,0X08,//偏绿
	0X1E,0xF0,0XF0,0X08,//过曝
	0X06,0x40,0X10,0X09,//正负片叠加
}; 
/**
  * @brief  特殊效果
  * @param  参数用于选择光线模式
	*   0x01  正常 
	*   0x02  冷色
	*   0x03  暖色
	*   0x04  黑白
	*   0x05  泛黄 
	*   0x06  反色
	*   0x07  偏绿
	*   0x08  过曝
	*   0x09  正负片叠加

  * @retval None
  */
void OV5640_SpecialEffects(uint8_t mode)
{
	OV5640_WriteReg(0x3212, 0x03); // start group 3
	
	OV5640_WriteReg(0x5580, OV5640_Effect_reg[mode][0]);
	OV5640_WriteReg(0x5583, OV5640_Effect_reg[mode][1]); // sat U
	OV5640_WriteReg(0x5584, OV5640_Effect_reg[mode][2]); // sat V
	OV5640_WriteReg(0x5003, OV5640_Effect_reg[mode][3]);
	
	OV5640_WriteReg(0x3212, 0x13); // end group 3
	OV5640_WriteReg(0x3212, 0xa3); // launch group 3
}


/**
  * @brief  设置光照度
	* @param  bri:光照度，参数范围[-4~+4]
  * @retval 无
  */
void OV5640_BrightnessConfig(int8_t Brightness)
{	
	OV5640_WriteReg(0x3212, 0x03); // start group 3
	
	OV5640_WriteReg(0x5587, (Brightness<<4)&0xF0);

	if(Brightness >= 0)	
	{
		OV5640_WriteReg(0x5588, 0x01);
	}
	else
	{
		OV5640_WriteReg(0x5588, 0x09);
	}
		
	OV5640_WriteReg(0x3212, 0x13); // end group 3
	OV5640_WriteReg(0x3212, 0xa3); // launch group 3
}

//对比度参数表
const static uint8_t OV5640_Contrast_reg[][2]=
{
 0x2C,0x1C,
 0x28,0x18,
 0x24,0x10,
 0x20,0x00,
 0x1C,0x1C,
 0x18,0x18,
 0x14,0x14,
};

/**
  * @brief  设置对比度
	* @param  cnst:对比度，参数范围[-3~+3]
  * @retval 无
  */
void OV5640_ContrastConfig(int8_t cnst)
{	
	OV5640_WriteReg(0x3212, 0x03); // start group 3
	OV5640_WriteReg(0x5586, OV5640_Contrast_reg[cnst+3][0]);
	OV5640_WriteReg(0x5585, OV5640_Contrast_reg[cnst+3][1]);
	OV5640_WriteReg(0x3212, 0x13); // end group 3
	OV5640_WriteReg(0x3212, 0xa3); // launch group 3
}

//色彩饱和度参数表
const static uint8_t OV5640_Saturation_reg[][6]=
{ 
	0X0C,0x30,0X3D,0X3E,0X3D,0X01,//-3 
	0X10,0x3D,0X4D,0X4E,0X4D,0X01,//-2  
	0X15,0x52,0X66,0X68,0X66,0X02,//-1  
	0X1A,0x66,0X80,0X82,0X80,0X02,//0  
	0X1F,0x7A,0X9A,0X9C,0X9A,0X02,//+1  
	0X24,0x8F,0XB3,0XB6,0XB3,0X03,//+2
	0X2B,0xAB,0XD6,0XDA,0XD6,0X04,//+3
}; 
/**
  * @brief  设置饱和度
  * @param  sat:饱和度,参数范围[-3 ~ +3]             	
  * @retval 无
  */
void OV5640_Color_Saturation(int8_t sat)
{
	uint8_t i;
	
	OV5640_WriteReg(0x3212, 0x03); // start group 3
	OV5640_WriteReg(0x5381, 0x1c);	
	OV5640_WriteReg(0x5382, 0x5a);
	OV5640_WriteReg(0x5383, 0x06);
	
	for(i=0;i<6;i++)
	{
		OV5640_WriteReg(0x5384+i, OV5640_Saturation_reg[sat+3][i]);
	}
	
	OV5640_WriteReg(0x538b, 0x98);
	OV5640_WriteReg(0x538a, 0x01);	
	OV5640_WriteReg(0x3212, 0x13); // end group 3
	OV5640_WriteReg(0x3212, 0xa3); // launch group 3
	
}			

//曝光补偿设置参数表
const static uint8_t OV5640_Exposure_reg[][6]=
{
    0x10,0x08,0x10,0x08,0x20,0x10,//-3  
    0x20,0x18,0x41,0x20,0x18,0x10,//-2
    0x30,0x28,0x61,0x30,0x28,0x10,//-1 
    0x38,0x30,0x61,0x38,0x30,0x10,//0  
    0x40,0x38,0x71,0x40,0x38,0x10,//+1 
    0x50,0x48,0x90,0x50,0x48,0x20,//+2   
    0x60,0x58,0xa0,0x60,0x58,0x20,//+3    
};

/**
  * @brief  设置曝光补偿
  * @param  ev:曝光补偿等级，参数范围[-3 ~ +3]             	
  * @retval 无
  */
void OV5640_Exposure(int8_t ev)
{
	OV5640_WriteReg(0x3212, 0x03); // start group 3

	OV5640_WriteReg(0x3a0f, OV5640_Exposure_reg[ev+3][0]);
	OV5640_WriteReg(0x3a10, OV5640_Exposure_reg[ev+3][1]);
	OV5640_WriteReg(0x3a11, OV5640_Exposure_reg[ev+3][2]);
	OV5640_WriteReg(0x3a1b, OV5640_Exposure_reg[ev+3][3]);
	OV5640_WriteReg(0x3a1e, OV5640_Exposure_reg[ev+3][4]);
	OV5640_WriteReg(0x3a1f, OV5640_Exposure_reg[ev+3][5]);
	
	OV5640_WriteReg(0x3212, 0x13); // end group 3
	OV5640_WriteReg(0x3212, 0xa3); // launch group 3

}

/**
  * @brief  使用cammode参数初始化各项配置
  * @param  None        	
  * @retval None
  */
void OV5640_USER_Config(void)
{	
	OV5640_FrameRate_Set(cam_mode.frame_rate);
	Delay(100);

	OV5640_ISPSize_Set(cam_mode.cam_isp_sx,
											 cam_mode.cam_isp_sy,
											 cam_mode.cam_isp_width,
											 cam_mode.cam_isp_height);
	Delay(100);

  OV5640_OutSize_Set(cam_mode.scaling,
												cam_mode.cam_out_sx,
												cam_mode.cam_out_sy,
												cam_mode.cam_out_width,
												cam_mode.cam_out_height);
	Delay(100);
	OV5640_BrightnessConfig(cam_mode.brightness);
	Delay(100);

	OV5640_Color_Saturation(cam_mode.saturation);
	Delay(100);

	OV5640_ContrastConfig(cam_mode.contrast);
	Delay(100);

	OV5640_Exposure(cam_mode.exposure);
	Delay(100);

	OV5640_LightMode(cam_mode.light_mode);
	Delay(100);

	OV5640_SpecialEffects(cam_mode.effect);	
	Delay(100);

//	Delay(500);
}

/**
  * @brief  获取时钟
  * @param  None        	
  * @retval 返回OV5640的系统时钟，返回值乘以10000后，单位为Hz
  */
int OV5640_get_sysclk(void)
{
	int XVCLK = 2400; // real clock/10000

	// calculate sysclk
	int temp1, temp2;
	int Multiplier, PreDiv, VCO, SysDiv, Pll_rdiv, Bit_div2x, sclk_rdiv, sysclk;
	int sclk_rdiv_map[] = {
	1, 2, 4, 8};
	temp1 = OV5640_ReadReg(0x3034);
	temp2 = temp1 & 0x0f;
	if (temp2 == 8 || temp2 == 10) {
	Bit_div2x = temp2 / 2;
	}
	temp1 = OV5640_ReadReg(0x3035);
	SysDiv = temp1>>4;
	if(SysDiv == 0) {
	SysDiv = 16;
		}
	temp1 = OV5640_ReadReg(0x3036);
	Multiplier = temp1;
	temp1 = OV5640_ReadReg(0x3037);
	PreDiv = temp1 & 0x0f;
	Pll_rdiv = ((temp1 >> 4) & 0x01) + 1;
	temp1 = OV5640_ReadReg(0x3108);
	temp2 = temp1 & 0x03;
	sclk_rdiv = sclk_rdiv_map[temp2];
	VCO = XVCLK * Multiplier / PreDiv;
	sysclk = VCO / SysDiv / Pll_rdiv * 2 / Bit_div2x / sclk_rdiv;
	return sysclk;
}


/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示IIC读取失败.
  */
static  uint8_t CAMERA_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  CAMERA_ERROR("Camera 等待超时!errorCode = %d",errorCode);

 	//产生结束信号，释放总线
	I2C_GenerateSTOP(CAMERA_I2C, ENABLE);

  return 0;
}

/**
  * @brief  写一字节数据到OV5640寄存器
  * @param  Addr: OV5640 的寄存器地址
  * @param  Data: 要写入的数据
  * @retval 返回0表示写入正常，0xFF表示错误
  */
uint8_t OV5640_WriteReg(uint16_t Addr, uint8_t Data)
{
	uint32_t timeout = DCMI_TIMEOUT_MAX;
  
	/* Generate the Start Condition */
	I2C_GenerateSTART(CAMERA_I2C, ENABLE);

	/* Test on CAMERA_I2C EV5 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(0);
	}
   
	/* Send DCMI selcted device slave Address for write */
	I2C_Send7bitAddress(CAMERA_I2C, OV5640_DEVICE_ADDRESS, I2C_Direction_Transmitter);
 
	/* Test on CAMERA_I2C EV6 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(1);
	}
 
	/* Send CAMERA_I2C location address MSB */
	I2C_SendData(CAMERA_I2C, (uint8_t)( (Addr >> 8) & 0xFF) );

	/* Test on CAMERA_I2C EV8 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(2);
	}
	//--------------------------------------------------------
	/* Send I2C1 location address LSB */
	I2C_SendData( CAMERA_I2C, (uint8_t)(Addr & 0xFF) );
	/* Test on I2C1 EV8 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(3);
	} 
  

	//--------------------------------------------------------

	/* Send Data */
	I2C_SendData(CAMERA_I2C, Data);    

	/* Test on CAMERA_I2C EV8 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(4);
	}  
 
	/* Send I2C1 STOP Condition */
	I2C_GenerateSTOP(CAMERA_I2C, ENABLE);
  
	/* If operation is OK, return 0 */
	return 0;
}
/**
  * @brief  从OV5640寄存器中读取一个字节的数据
  * @param  Addr: 寄存器地址
  * @retval 返回读取得的数据
  */
uint8_t OV5640_ReadReg(uint16_t Addr)
{
	uint32_t timeout = DCMI_TIMEOUT_MAX;
	uint8_t Data = 0;

	/* Generate the Start Condition */
	I2C_GenerateSTART(CAMERA_I2C, ENABLE);

	/* Test on CAMERA_I2C EV5 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(5);
	} 
  
	/* Send DCMI selcted device slave Address for write */
	I2C_Send7bitAddress(CAMERA_I2C, OV5640_DEVICE_ADDRESS, I2C_Direction_Transmitter);
 
	/* Test on I2C1 EV6 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(6);
	} 

	/* Send I2C1 location address MSB */
	I2C_SendData( CAMERA_I2C, (uint8_t)((Addr>>8) & 0xFF) );

	/* Test on I2C1 EV8 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(7);
	} 
  
	/* Clear AF flag if arised */
	CAMERA_I2C->SR1 |= (uint16_t)0x0400; 
  
//--------------------------------------------------------
	/* Send I2C1 location address LSB */
	I2C_SendData( CAMERA_I2C, (uint8_t)(Addr & 0xFF) );
	/* Test on I2C1 EV8 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(8);
	} 
  
	/* Clear AF flag if arised */
	CAMERA_I2C->SR1 |= (uint16_t)0x0400;
	//--------------------------------------------------------

	/* Generate the Start Condition */
	I2C_GenerateSTART(CAMERA_I2C, ENABLE);
  
	/* Test on I2C1 EV6 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(9);
	} 
  
	/* Send DCMI selcted device slave Address for write */
	I2C_Send7bitAddress(CAMERA_I2C, OV5640_DEVICE_ADDRESS, I2C_Direction_Receiver);
   
	/* Test on I2C1 EV6 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(10);
	}  
 
	/* Prepare an NACK for the next data received */
	I2C_AcknowledgeConfig(CAMERA_I2C, DISABLE);
	
	/* Test on I2C1 EV7 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(11);
	}   
    
	/* Prepare Stop after receiving data */
	I2C_GenerateSTOP(CAMERA_I2C, ENABLE);

	/* Receive the Data */
	Data = I2C_ReceiveData(CAMERA_I2C);

	/* return the read data */
	return Data;
}


/**
  * @brief  将固件写入到OV5640片内MCU
  * @param  Addr: OV5640 的MCU基地址0x8000
  * @param  Data: 要写入的数据
  * @retval 返回0表示写入正常，0xFF表示错误
  */
uint8_t OV5640_WriteFW(uint8_t *pBuffer ,uint16_t BufferSize)
{
	uint32_t timeout = DCMI_TIMEOUT_MAX;
    uint16_t Addr=0x8000;
    
	/* Generate the Start Condition */
	I2C_GenerateSTART(CAMERA_I2C, ENABLE);

	/* Test on CAMERA_I2C EV5 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(12);
	}
   
	/* Send DCMI selcted device slave Address for write */
	I2C_Send7bitAddress(CAMERA_I2C, OV5640_DEVICE_ADDRESS, I2C_Direction_Transmitter);
 
	/* Test on CAMERA_I2C EV6 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(13);
	}
 
	/* Send CAMERA_I2C location address MSB */
	I2C_SendData(CAMERA_I2C, (uint8_t)( (Addr >> 8) & 0xFF) );

	/* Test on CAMERA_I2C EV8 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(14);
	}
	//--------------------------------------------------------
	/* Send I2C1 location address LSB */
	I2C_SendData( CAMERA_I2C, (uint8_t)(Addr & 0xFF) );
	/* Test on I2C1 EV8 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(15);
	} 
  
	//--------------------------------------------------------
/* While there is data to be written */
  while(BufferSize--)  
  {
    /* Send Data */
    I2C_SendData(CAMERA_I2C, *pBuffer);  
      
    /* Point to the next byte to be written */
    pBuffer++; 
    /* Test on CAMERA_I2C EV8 and clear it */
    timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        if ((timeout--) == 0) return CAMERA_TIMEOUT_UserCallback(16);
    }  
   }
	/* Send I2C1 STOP Condition */
	I2C_GenerateSTOP(CAMERA_I2C, ENABLE);
  
	/* If operation is OK, return 0 */
	return 0;
}

/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
