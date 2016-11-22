#include <GLGPU.h>
#include <string>
#include <iostream>
#include <iterator>


static const std::string vertex_shader_source =
      "#version 330\nin vec2 position;\nin vec3 color;\nout vec3 frag_color;\nvoid main()\n{\nfrag_color = color;\ngl_Position = vec4(position, 0.0, 1.0);\n}";

static const std::string fragment_shader_source = 
      "#version 330\nin vec3 frag_color;\nout vec4 outColor;\nvoid main()\n{\noutColor = vec4(frag_color, 1.0);\n}";

// TODO: gpu errors
glgpu::glgpu()
{
      // initialise OpenGL
      if (!glfwInit())
      {
            // throw error
      }

      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      window = glfwCreateWindow(320, 240, "PSXEmu", NULL, NULL);
      glfwMakeContextCurrent(window);
      
      glewExperimental = true;
      if (glewInit() != GLEW_OK)
      {
            // throw error
      }

      glGenVertexArrays(1, &vertex_array);
      glBindVertexArray(vertex_array);

      glGenBuffers(1, &vertex_buffer);
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
      // buffer may need to be setup later?
      //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat[100000]), NULL, GL_STREAM_DRAW);

      gourad_shader = compileShader(vertex_shader_source.c_str(), fragment_shader_source.c_str());
      glUseProgram(gourad_shader);

      GLint position_attr = glGetAttribLocation(gourad_shader, "position");
      glEnableVertexAttribArray(position_attr);
      glVertexAttribPointer(position_attr, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);

      GLint colour_attr = glGetAttribLocation(gourad_shader, "color");
      glEnableVertexAttribArray(colour_attr);
      glVertexAttribPointer(colour_attr, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));


      // registers & function pointer(s)

      /*gp0_command = {
            &glgpu::RESERVED, &glgpu::clearCache, &glgpu::frameBufferRectDraw, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,

            &glgpu::mono3Polygon, &glgpu::RESERVED, &glgpu::mono3Polygon, &glgpu::RESERVED,
            &glgpu::tex3Polygon, &glgpu::tex3Polygon, &glgpu::tex3Polygon, &glgpu::tex3Polygon,
            &glgpu::mono4Polygon, &glgpu::RESERVED, &glgpu::mono4Polygon, &glgpu::RESERVED,
            &glgpu::tex4Polygon, &glgpu::tex4Polygon, &glgpu::tex4Polygon, &glgpu::tex4Polygon,
            &glgpu::grad3Polygon, &glgpu::RESERVED, &glgpu::grad3Polygon, &glgpu::RESERVED,
            &glgpu::gradTex3Polygon, &glgpu::RESERVED, &glgpu::gradTex3Polygon, &glgpu::RESERVED,
            &glgpu::grad4Polygon, &glgpu::RESERVED, &glgpu::grad4Polygon, &glgpu::RESERVED,
            &glgpu::gradTex4Polygon, &glgpu::RESERVED, &glgpu::gradTex4Polygon, &glgpu::RESERVED,

            &glgpu::monoLine, &glgpu::RESERVED, &glgpu::monoLine, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::monoPolyLine, &glgpu::RESERVED, &glgpu::monoPolyLine, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::gradLine, &glgpu::RESERVED, &glgpu:gradLine, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::gradPolyLine, &glgpu::RESERVED, &glgpu::gradPolyLine, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,

            &glgpu::rect, &glgpu::RESERVED, &glgpu::rect, &glgpu::RESERVED,
            &glgpu::sprite, &glgpu::sprite, &glgpu::sprite, &glgpu::sprite,
            &glgpu::dot, &glgpu::RESERVED, &glgpu::dot, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::rect8, &glgpu::RESERVED, &glgpu::rect8, &glgpu::RESERVED,
            &glgpu::sprite8, &glgpu::sprite8, &glgpu::sprite8, &glgpu::sprite8,
            &glgpu::rect16, &glgpu::RESERVED, &glgpu::rect16, &glgpu::RESERVED,
            &glgpu::sprite16, &glgpu::sprite16, &glgpu::sprite16, &glgpu::sprite16,

            &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, 
            &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, 
            &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, 
            &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, 
            &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, 
            &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, 
            &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, 
            &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, &glgpu::copyInVRAM, 

            &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, 
            &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, 
            &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, 
            &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, 
            &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, 
            &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, 
            &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, 
            &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, &glgpu::copyToVRAM, 

            &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, 
            &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, 
            &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, 
            &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, 
            &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, 
            &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, 
            &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, 
            &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, &glgpu::copyFromVRAM, 

            &glgpu::RESERVED, &glgpu::drawMode, &glgpu::textureWindow, &glgpu::setDrawTopLeft,
            &glgpu::setDrawBottomRight, &glgpu::drawingOffset, &glgpu::maskSetting, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED,
            &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED, &glgpu::RESERVED
      };*/

      //gp1_command

      // other status
      params_rem = 0;
      //command_buffer = FIFOImpl<word>(16);
      refresh_ready = false;
      glfwSetTime(0);
      total_time = 0;
      total_poly = 0;

      draw_x_left = 0;
      draw_x_right = 320;
      draw_y_top = 0;
      draw_y_bottom = 240;
      offset_x = 0;
      offset_y = 0;
      glClear(GL_COLOR_BUFFER_BIT);
}

glgpu::~glgpu()
{
      glDeleteVertexArrays(1, &vertex_array);
      glDeleteBuffers(1, &vertex_buffer);
      glDeleteProgram(gourad_shader);

      glfwTerminate();
}

bool glgpu::refreshWindow()
{
      glfwPollEvents();
      if (glfwWindowShouldClose(window) || glfwGetTime() >= 100.0)
      {
            std::cout << total_poly / 100 << std::endl;
            glfwDestroyWindow(window);
            return false;
      }

      //while(refresh_ready)
      //{
            if (glfwGetTime() >= 0.0333 + total_time)
            {
                  total_time += 0.0333;
                  refresh_ready = false;
                  glBufferData(GL_ARRAY_BUFFER, triangle_buffer.size()*sizeof(GLfloat), triangle_buffer.data(), GL_STATIC_DRAW);
                  glDrawArrays(GL_TRIANGLES, 0, 3*triangle_count);
                  triangle_count = 0;
                  triangle_buffer.clear();
                  glfwSwapBuffers(window);
                  glClear(GL_COLOR_BUFFER_BIT);
            }
      //}

      return true;
}

// interface functions
void glgpu::wordFromDMA(word in)
{
      if (params_rem > 0)
      {
            command_buffer.write(in);
            params_rem--;
            if (params_rem == 0)
                  //gp0[gp0_command](this);
                  mono3Polygon();
      }
      else
      {
            gp0_command = (in >> 24) & 0xFF;
            params_rem = /*gp0[gp0_command](this);*/ mono3Polygon();
      }
}

word glgpu::wordToDMA()
{
      return gpu_response;
}

word glgpu::readWord(unsigned address)
{
      if (address % 2 == 0)
      {
            return gpu_response;
      }
      else
      {
            return 0;
            // make and return GPUstat
      }
}

void glgpu::writeWord(unsigned address, word in)
{
      if (address % 2 == 0)
      {
            // gp0 command
            if (params_rem > 0)
            {
                  command_buffer.write(in);
                  params_rem--;
                  if (params_rem == 0)
                        //gp0[(gp0_command >> 24) & 0xFF](this);
                        mono3Polygon();
            }
            else
            {
                  gp0_command = in;
                  params_rem = /*gp0[(gp0_command >> 24) & 0xFF](this);*/ mono3Polygon();
            }
      }
      else
      {
            // gp1 command
            //gp1[(in >> 24) % 0x20](this, in);
      }
}



GLuint glgpu::compileShader(const char* vertex_shader_source, const char* fragment_shader_source)
{
      GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
      glCompileShader(vertex_shader);
      GLint status;
      glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
      if (status != GL_TRUE)
      {
            std::cout << "shader fuckup 1" << std::endl;
            // throw error
      }

      GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
      glCompileShader(fragment_shader);
      glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
      if (status != GL_TRUE)
      {
            std::cout << "shader fuckup 2" << std::endl;
            // throw error
      }
      
      GLuint shader_program = glCreateProgram();
      glAttachShader(shader_program, vertex_shader);
      glAttachShader(shader_program, fragment_shader);

      glLinkProgram(shader_program);

      glDetachShader(shader_program, vertex_shader);
      glDetachShader(shader_program, fragment_shader);
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);

      return shader_program;
}

GLfloat glgpu::convertXCoord(s_halfword in)
{
      // get co-ords normalised to 0 at left.
      in += offset_x;
      // get co-ords normalised to 0 in centre.
      in -= ((draw_x_right - draw_x_left) / 2);
      // get co-ords normalised to -1.0, 1.0
      GLfloat out = GLfloat(in) / ((draw_x_right - draw_x_left) / 2);

      if (out < -1.0)
            out = -1.0;
      else if (out > 1.0)
            out = 1.0;

      return out;
}

GLfloat glgpu::convertYCoord(s_halfword in)
{
      // get co-ords normalised to 0 at left.
      in += offset_y;
      // get co-ords normalised to 0 in centre.
      in -= ((draw_y_bottom - draw_y_top) / 2);
      // get co-ords normalised to -1.0, 1.0
      GLfloat out = GLfloat(in) / ((draw_y_bottom - draw_y_top) / 2);

      if (out < -1.0)
            out = -1.0;
      else if (out > 1.0)
            out = 1.0;

      return out;
}
/*
      x x 1 0 0 0 0 0 0 0
      x x 0 0 0 0 0 0 0 0
      x x 0 0 0 0 0 0 0 0

in : (-3, 1) -> (0, 0)
draw_top_left = 2,0
draw_bottom_right = 9,2
(0, 0) -> (-3, 1)
in / draw_x_right - draw_x

0,0 -> -1.0, 1.0

*/

GLfloat glgpu::convertColour(byte in)
{
      // 0-15 -> 0.0-0.66, 16-31 -> 0.66-1.0
      GLfloat result = in / 23.0;

      if (result > 0.67)
      {
            in -= 16;
            result = in / 48.0;
            result += 0.6875;
      }

      return result;
}


void glgpu::setWindowResolution()
{
      unsigned hor_res;
      unsigned vert_res;

      if (horizontal_resolution_2 == 1)
      {
            hor_res = 368;
      }
      else
      {
            if (horizontal_resolution_1 == 0)
            {
                  hor_res = 256;
            }
            else if (horizontal_resolution_1 == 1)
            {
                  hor_res = 320;
            }
            else if (horizontal_resolution_1 == 2)
            {
                  hor_res = 512;
            }
            else
            {
                  hor_res = 640;
            }
      }

      if (vertical_resolution == 0)
      {
            vert_res = 240;
      }
      else
      {
            vert_res = 480;
      }

      glfwSetWindowSize(window, hor_res, vert_res);
}


// control functions
void glgpu::resetGPU(unsigned param)
{
      command_buffer.clear();
      interrupt_request = 0;
      disable_display = 1;
      // dma off
      x_display = 0;
      y_display = 0;
      // display ranges reset
      horizontal_resolution_1 = 0;
      vertical_resolution = 0;
      video_mode = 0;
      colour_depth = 0;
      vertical_interlace = 0;
      horizontal_resolution_2 = 0;
      reverse_flag = 0;
      // E1- E6 = 0
}

void glgpu::resetCommandBuffer(unsigned param)
{
      command_buffer.clear();
}

void glgpu::resetIRQ(unsigned param)
{
      interrupt_request = 0;
}

void glgpu::displayEnable(unsigned param)
{
      disable_display = param & 0x1;
}

void glgpu::setupDMA(unsigned param)
{
      // dma stuff
}

void glgpu::displayArea(unsigned param)
{
      x_display = param & 0x3FF;
      y_display = (param >> 10) & 0x1FF;
      // setWindowResolution?
}

void glgpu::horizontalDisplayRange(unsigned param)
{
      // not sure if this is necessary.

      //unsigned x1 = param & 0xFFF;
      //unsigned x2 = (param >> 12) & 0xFFF;
}

void glgpu::verticalDisplayRange(unsigned param)
{
      // not sure if this is necessary.
}

void glgpu::displayMode(unsigned param)
{
      horizontal_resolution_1 = param & 0x3;
      vertical_resolution = (param >> 2) & 0x1;
      video_mode = (param >> 3) & 0x1;
      colour_depth = (param >> 4) & 0x1;
      vertical_interlace = (param >> 5) & 0x1;
      horizontal_resolution_2 = (param >> 6) & 0x1;
      reverse_flag = (param >> 7) & 0x1;

      // set window res?
}

void glgpu::gpuInfo(unsigned param)
{
      switch (param & 0xF)
      {
      case 2:
            gpu_response = tex_mask_x & 0x1F;
            gpu_response |= (tex_mask_y & 0x1F) << 5;
            gpu_response |= (tex_offset_x & 0x1F) << 10;
            gpu_response |= (tex_offset_y & 0x1F) << 15;
            break;
      case 3:
            gpu_response = draw_x_left & 0x3FF;
            gpu_response |= (draw_y_top & 0x3FF) << 10;
            break;
      case 4:
            gpu_response = draw_x_right & 0x3FF;
            gpu_response |= (draw_y_bottom & 0x3FF) << 10;
            break;
      case 5:
            gpu_response = offset_x & 0x7FF;
            gpu_response |= (offset_y & 0x7FF) << 11;
            break;
      case 7:
            gpu_response = 2; // gpu type
            break;
      case 8:
            gpu_response = 0;
      default:
            break;
      }
}


unsigned RESERVED()
{
      return 0;
}




// primitive drawing
unsigned glgpu::mono3Polygon()
{
      if (command_buffer.size() < 3)
            return 3;

      // retrieve arguments
      word vertex_1 = command_buffer.read();
      word vertex_2 = command_buffer.read();
      word vertex_3 = command_buffer.read();

      // convert x & y input to float. likely a complex function of its own
      GLfloat x_1 = convertXCoord(s_halfword(vertex_1 & 0xFFFF));
      GLfloat y_1 = convertYCoord(s_halfword((vertex_1 >> 16) & 0xFFFF));
      GLfloat x_2 = convertXCoord(s_halfword(vertex_2 & 0xFFFF));
      GLfloat y_2 = convertYCoord(s_halfword((vertex_2 >> 16) & 0xFFFF));
      GLfloat x_3 = convertXCoord(s_halfword(vertex_3 & 0xFFFF));
      GLfloat y_3 = convertYCoord(s_halfword((vertex_3 >> 16) & 0xFFFF));

      // convert colour
      GLfloat r = convertColour((gp0_command >> 3) & 0x1F);
      GLfloat g = convertColour((gp0_command >> 11) & 0x1F);
      GLfloat b = convertColour((gp0_command >> 19) & 0x1F);

      // add to triangle buffer.
      GLfloat triangle[] = {  x_1, y_1, r, g, b,
                              x_2, y_2, r, g, b,
                              x_3, y_3, r, g, b };
      triangle_buffer.insert(triangle_buffer.end(), triangle, std::end(triangle));

      //glBufferSubData(GL_ARRAY_BUFFER, tri_ptr, sizeof(triangle), triangle);
      //glDrawArrays(GL_TRIANGLES, 0, 3);

      // use shader (maybe not until render time?)
            // maybe make use of vao here.
      // might need to store meta data about triangle, eg:
            // primitive_list->add(triangle)
            // vertex_count += 3

      triangle_count++;
      total_poly++;
      refresh_ready = true;

      return 0;
}
















// gp0_command & transfer packets
unsigned glgpu::clearCache()
{
      // do nothing?
      // glClearBufferData(?)
      return 0;
}

unsigned glgpu::frameBufferRectDraw()
{
      // convert values to float,
      // render rectangle in buffer
      return 0;
}

unsigned glgpu::copyInVRAM()
{
      // check WHERE in frame buffer is being accessed
      // tex->tex: adjust texture page
      // frame->frame: (do nothing? or render to screen?)
      // other: may need to access glFrameBuffer
      return 0;
}

unsigned glgpu::copyToVRAM()
{
      return 0;
}

unsigned glgpu::copyFromVRAM()
{
      return 0;
}


// draw mode/environment
unsigned glgpu::drawMode()
{
      texture_page_x_base = gp0_command & 0xF;
      texture_page_y_base = (gp0_command >> 4) & 0x1;
      semi_transparency = (gp0_command >> 5) & 0x3;
      texture_page_colours = (gp0_command >> 7) & 0x3;
      dither_enable = (gp0_command >> 9) & 0x1;
      draw_to_display_area = (gp0_command >> 10) & 0x1;
      texture_disable = (gp0_command >> 11) & 0x1;
      // x and y flip?
      return 0;
}

unsigned glgpu::textureWindow()
{
      tex_mask_x = gp0_command & 0x1F;
      tex_mask_y = (gp0_command >> 5) & 0x1F;
      tex_offset_x = (gp0_command >> 10) & 0x1F;
      tex_offset_y = (gp0_command >> 15) & 0x1F;
      return 0;
}

unsigned glgpu::setDrawTopLeft()
{
      draw_x_left = gp0_command & 0x3FF;
      draw_y_top = (gp0_command >> 10) & 0x1FF;
      // viewport?
      return 0;
}

unsigned glgpu::setDrawBottomRight()
{
      draw_x_right = gp0_command & 0x3FF;
      draw_y_bottom = (gp0_command >> 10) & 0x1FF;
      // viewport?
      return 0;
}

unsigned glgpu::drawingOffset()
{
      offset_x = gp0_command & 0x7FF;
      offset_x = (offset_x << 21) >> 21;
      offset_y = (gp0_command >> 11) & 0x7FF;
      offset_y = (offset_y << 21) >> 21;
      return 0;
}

unsigned glgpu::maskSetting()
{
      set_mask_bit = gp0_command & 0x1;
      draw_pixels = (gp0_command >> 1) & 0x1;
      return 0;
}






