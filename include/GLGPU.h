#ifndef GL_GPU_H
#define GL_GPU_H

/*
 *    GLGPU.h
 *
 *    Declares a GPU. Using OpenGL
 *
 */

// includes
//#include <GL/glew.h>
//#include <glfw3.h>
#include <Memory.h>
#include <DMA.h>
#include <FIFO.h>
#include <vector>
#include <functional>


// the gpu is in charge of all video output.
// therefore it manages the window.
// in charge of a lot: interacts with other hardware thru CPU & DMA, and directly outputs video
class glgpu : public dmaDevice, public memoryInterface
{
      // opengl stuff
      // output window
      GLFWwindow* window;
      // buffers for polygons
      GLuint vertex_array;
      GLuint vertex_buffer;
      // shader programs
      GLuint gourad_shader;

      // status register
      unsigned texture_page_x_base;
      unsigned texture_page_y_base;
      unsigned semi_transparency;
      unsigned texture_page_colours;
      bool dither_enable;
      bool draw_to_display_area;
      bool set_mask_bit;
      bool draw_pixels;
      bool reverse_flag;
      bool texture_disable;
      unsigned horizontal_resolution_1;
      unsigned horizontal_resoltuion_2;
      unsigned vertical_resolution;
      bool video_mode;
      bool colour_depth;
      bool vertical_interlace;
      bool disable_display;
      bool interrupt_request;

      // dma stuff

      // function pointers
      std::vector<std::function<unsigned(glgpu*)>> gp0;
      std::vector<std::function<void(glgpu*, unsigned)>> gp1;
      // number of parameters to wait for.
      unsigned params_rem;


      // other status
      // current command
      word gp0_command;
      //word gp1_command;
      FIFOImpl<word> command_buffer;
      // texture window mask
      unsigned tex_mask_x;
      unsigned tex_mask_y;
      unsigned tex_offset_x;
      unsigned tex_offset_y;
      // drawing area
      unsigned draw_x_left;
      unsigned draw_x_right;
      unsigned draw_y_top;
      unsigned draw_y_bottom;
      // drawing offset
      signed offset_x;
      signed offset_y;
      // VRAM
      unsigned x_display;
      unsigned y_display;
      // GPU return
      word gpu_response;




public:
      glgpu();
      ~glgpu();

      // System interaction functions
      // things like initialise, refresh window, etc.

      // DMA functions
      void wordFromDMA(word in) override;
      word wordToDMA() override;

      // memoryInterface functions
      word readWord(unsigned address) override;
      void writeWord(unsigned address, word in) override;

private:
      // internal gl functions
      // compiles shader code into program
      GLuint compileShader(const char* vertex_shader_source, const char* fragment_shader_source);
      // converts 16-bit pixel co-ord into float (-1.0 to 1.0)
      GLfloat convertXCoord(s_halfword in);
      GLfloat convertYCoord(s_halfword in);
      // converts 8-bit colour component to float (-1.0 to 1.0)
      GLfloat convertColour(byte in);

      // sets window
      void setWindowResolution();
      //void setDrawArea();


      // control functions
      void resetGPU(unsigned param);
      void resetCommandBuffer(unsigned param);
      void resetIRQ(unsigned param);
      void displayEnable(unsigned param);
      void setupDMA(unsigned param);
      void displayArea(unsigned param);
      void horizontalDisplayRange(unsigned param);
      void verticalDisplayRange(unsigned param);
      void displayMode(unsigned param);
      void gpuInfo(unsigned param);

      // gp0 commands
      // these all return the number of parameters needed to run
      unsigned RESERVED();

      // primitive drawing
      unsigned mono3Polygon();
      unsigned tex3Polygon();
      unsigned grad3Polygon();
      unsigned gradTex3Polygon();

      unsigned mono4Polygon();
      unsigned tex4Polygon();
      unsigned grad4Polygon();
      unsigned gradTex4Polygon();

      unsigned monoLine();
      unsigned monoPolyLine();
      unsigned gradLine();
      unsigned gradPolyLine();

      unsigned rect();
      unsigned sprite();
      unsigned dot();

      unsigned rect8();
      unsigned sprite8();
      unsigned rect16();
      unsigned sprite16();

      // command & transfer packets
      unsigned clearCache();
      unsigned frameBufferRectDraw();
      unsigned copyInVRAM();
      unsigned copyToVRAM();
      unsigned copyFromVRAM();

      // draw mode/environment
      unsigned drawMode();
      unsigned textureWindow();
      unsigned setDrawTopLeft();
      unsigned setDrawBottomRight();
      unsigned drawingOffset();
      unsigned maskSetting();

};


#endif
