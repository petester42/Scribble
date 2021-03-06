/* 
 * File:   Sender.h
 * Author: scribble
 *
 * Created on October 30, 2012, 7:55 PM
 */

#ifndef SENDER_H
#define	SENDER_H

#include <string>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
#include <sstream>
#include <vector>
#include "Point.h"

class Sender
{
public:
    Sender(std::string, int);
    //    Sender(const Sender& orig);
    virtual ~Sender();

    static std::string getSplit();

    //login - username - requestID - password - port
    void Login(std::string username, std::string password, int myListeningPort);

    //logout - username - requestID
    void Logout();

    //requestOwnership - username - requestID
    void RequestOwnership();

    //releaseOwnership - username - requestID
    void ReleaseOwnership();

    //getFileList - username - requestID
    void GetFilesList();

    //TOCONFIRM Do we Need this?
    void DownloadFile(std::string filename);

    //newPath - username - requestID - pathID - mode - color - active - page
    void NewPath(int pathID, bool mode, int color, bool active, int page);

    //AddPoints - username - requestID - pathID - numberOfPoints - Points
    void AddPoints(int pathID, int numberOfPoints, std::vector<Point> points);

    //EndPath - username - requestID - pathID
    void EndPath(int pathID);

    //Undo - username - requestID - page - pathID
    void Undo(int page, int pathID);

    //Redo - username - requestID - page - pathID
    void Redo(int page, int pathID);

    //Delete - username - requestID - page - pathID 
    void DeletePath(int page, int PathID);

    static const std::string separator; // = "&";
private:
    void SendMessage(std::string toSend);
    std::string GetPoints(std::vector<Point> points);

    std::string NumberToString(int Number);
    std::string BoolToString(bool boolean);

    int requestID;

    const std::string separatorPoints;
    std::string serverName;
    int portno;

    //Need to initialise this in login
    std::string username;

    enum
    {
        LOGIN = 0,
        LOGOUT = 1,
        REQUEST_OWNERSHIP = 2,
        RELEASE_OWNERSHIP = 3,
        GET_FILES_LIST = 4,
        DOWNLOAD_FILE = 5, //TOCONFIRM do we need this?
        NEW_PATH = 6,
        ADD_POINTS = 7,
        END_PATH = 8,
        UNDO = 9,
        REDO = 10,
        DELETE_PATH = 11
    };

};

#endif	/* SENDER_H */