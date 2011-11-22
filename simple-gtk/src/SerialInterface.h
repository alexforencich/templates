/************************************************************************/
/* SerialInterface                                                      */
/*                                                                      */
/* Template - Serial Port Interface                                     */
/*                                                                      */
/* SerialInterface.h                                                    */
/*                                                                      */
/* Alex Forencich <alex@alexforencich.com>                              */
/*                                                                      */
/* Copyright (c) 2011 Alex Forencich                                    */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files(the "Software"), to deal in the Software without restriction,  */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* The above copyright notice and this permission notice shall be       */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  */
/* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   */
/* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    */
/* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     */
/* SOFTWARE.                                                            */
/*                                                                      */
/************************************************************************/

#ifndef __SERIALINTERFACE_H
#define __SERIALINTERFACE_H

#include <string>
#include <vector>
#include <gtkmm.h>

#ifdef __unix__
#include <termios.h>
#elif defined _WIN32
#include <windows.h>
#endif

#define SI_SUCCESS 0
#define SI_ERROR 1
#define SI_TIMEOUT 2
#define SI_PORT_NOT_OPEN 3

#define SI_FLOW_NONE 0
#define SI_FLOW_HARDWARE 1
#define SI_FLOW_XON_XOFF 2

#define SI_PARITY_NONE 0
#define SI_PARITY_ODD 1
#define SI_PARITY_EVEN 2

// class SerialInterface
class SerialInterface
{
public:
        SerialInterface();
        virtual ~SerialInterface();
        
        int write(const char *buf, gsize count, gsize& bytes_written);
        int read(char *buf, gsize count, gsize& bytes_read);
        
        int open_port();
        int close_port();
        
        bool is_open();
        
        Glib::ustring set_port(Glib::ustring p);
        Glib::ustring get_port();
        unsigned long set_baud(unsigned long b);
        unsigned long get_baud();
        int set_bits(int b);
        int get_bits();
        int set_flow(int f);
        int get_flow();
        int set_parity(int p);
        int get_parity();
        int set_stop(int s);
        int get_stop();
        
        bool set_debug(bool d);
        bool get_debug();
        
        Glib::ustring get_status_string();
        
        static std::vector<std::string> enumerate_ports();
        
        sigc::signal<void> port_opened();
        sigc::signal<void> port_closed();
        sigc::signal<void> port_error();
        sigc::signal<void> port_receive_data();
        
protected:
        void on_receive_data();
        void select_thread();
        void launch_select_thread();
        void stop_select_thread();
        void configure_port();
        
        //sigc::signal<void> signal_receive_data;
        Glib::Dispatcher signal_receive_data;
        
        #ifdef __unix__
        
        int port_fd;
        struct termios port_termios;
        struct termios port_termios_saved;
        
        #elif defined _WIN32
        
        HANDLE h_port;
        DCB dcb_serial_params;
        DCB dcb_serial_params_saved;
        HANDLE h_overlapped;
        HANDLE h_overlapped_thread;
        
        #endif
        
        Glib::Mutex running_mutex;
        Glib::Mutex read_mutex;
        Glib::Cond read_cond;
        Glib::Thread *thread;
        bool running;
        
        Glib::ustring port;
        unsigned long baud;
        int bits;
        int flow;
        int parity;
        int stop;
        
        bool debug;
        
        sigc::signal<void> m_port_opened;
        sigc::signal<void> m_port_closed;
        sigc::signal<void> m_port_error;
        sigc::signal<void> m_port_receive_data;
};

#endif //__SERIALINTERFACE_H


