#pragma once
#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;
static STARTUPINFO sti = { 0 };
static SECURITY_ATTRIBUTES sats = { 0 };
static PROCESS_INFORMATION pi = { 0 };
static HANDLE pipin_w, pipin_r, pipout_w, pipout_r;
static BYTE buffer[100000];
static DWORD writ, excode, read, available;
static string temp;
namespace Connector
{
    
    void ConnectToEngine(char* path);
    
    string getNextMove(string pos);
    
    void CloseConnection();
   
}
#endif CONNECTOR_H