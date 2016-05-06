/*
    Thread, multithreading library
    Copyright (C) 2016  Joseph Cluett

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Author(s):
        Joseph Cluett (main author)

    File Type: header, Thread

    Date Created: 2/4/2016

    Date Last Modified: 2/4/2016

    Purpose:
        Make the creation/use/destruction of threads much easier

    Misc. Notes:
        -) If you make changes to this library, please put your name in the above 'Author(s)'
        tab and put a description of your changes here in the 'Misc. Notes' tab
        -) This library is not cross-platform as it makes several linux-specific system calls.
        Library functions mostly as a wrapper for the pthreads library, while adding object
        orientation for future revisions
        -) Functions used in Thread objects must return void* and must accept void* as argument
        remember that void* can be cast to any other type of pointer
        -) Future revisions of this library will allow user to pass arbitrary number and type
        of arguments to Thread object in a linked list.
*/

#ifndef THREAD_JJC_H
#define THREAD_JJC_H

#include <iostream>
#include <pthread.h>

namespace JJC {

template<class T> //T describes type of pointer passed to thread, set to void to not pass any data at all
class Thread {
    public:
        Thread(void);

        //call one of these two constructors:
        Thread(void* (*f_ptr)(void*), T* t_data);   //one argument, a pointer
        Thread(void* (*f_ptr)(void*));              //assumes void argument

        //use one of these functions before calling thread start
        void setFunction(void* (*f_ptr)(void*), T* t_data);
        void setFunction(void* (*f_ptr)(void*));

        void forkThread(void);   //forks thread
        void syncThread(void);   //waits for thread to finish execution
        void haltThread(void);   //kills thread
        void pauseThread(void);  //pauses the thread to resume later
        void resumeThread(void); //resumes thread that was paused

    private:
        pthread_t* pid;
        void* (*fncPtr_l)(void*); //for functions that take an argument
        void* (*fncPtr_v)();
        T* f_ptr_param;
        bool call_with_data = false;
        bool call_wo_data = false;
        bool f_ptr_set = false; // dont call pthread_create unless this is set
};

}

template<class T>
JJC::Thread<T>::Thread(void) { //default constructor
    std::cout << "Default constructor used, not recommended" << std::endl;
    std::cout << "Must call thread setup method before forking" << std::endl;
}

template<class T>
JJC::Thread<T>::Thread(void* (*f_ptr)(void*), T* t_data) {
    setFunction(f_ptr, t_data);
}

template<class T>
JJC::Thread<T>::Thread(void* (*f_ptr)(void*)) {
    setFunction(f_ptr);
}

template<class T>
void JJC::Thread<T>::setFunction(void* (*f_ptr)(void*), T* t_data) {
    call_with_data = true;
    call_wo_data = false;
    f_ptr_set = true;

    //assign function pointer
    fncPtr_l = f_ptr;
    f_ptr_param = t_data;
}

template<class T>
void JJC::Thread<T>::setFunction(void* (*f_ptr)(void*)) {
    call_wo_data = true;
    call_with_data = false;
    f_ptr_set = true;

    //assign function pointer
    fncPtr_l = f_ptr;
    f_ptr_param = NULL;
}

template<class T>
void JJC::Thread<T>::forkThread(void) {
    if(call_with_data && !call_wo_data && f_ptr_set) { //fork thread with data
        pid = new pthread_t;
        pthread_create(pid, NULL, fncPtr_l, f_ptr_param);
    } else if(call_wo_data && !call_with_data && f_ptr_set) { //fork thread without data
        pid = new pthread_t;
        pthread_create(pid, NULL, fncPtr_l, NULL);
    } else {
        std::cout << "Error forking thread" << std::endl;
    }
}

template<class T>
void JJC::Thread<T>::syncThread(void) {
    pthread_join(*pid, NULL);
}

template<class T>
void JJC::Thread<T>::haltThread(void) {
    pthread_cancel(*pid);
}

template<class T>
void JJC::Thread<T>::pauseThread(void) {
    ;
}

template<class T>
void JJC::Thread<T>::resumeThread(void) {
    ;
}

#endif // THREAD_JJC_H
