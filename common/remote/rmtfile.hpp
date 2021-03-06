/*##############################################################################

    HPCC SYSTEMS software Copyright (C) 2012 HPCC Systems®.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
############################################################################## */

#ifndef RMTFILE_HPP
#define RMTFILE_HPP

#include "jsocket.hpp"
#include "jfile.hpp"

#include "sockfile.hpp"

#ifdef REMOTE_EXPORTS
#define REMOTE_API DECL_EXPORT
#else
#define REMOTE_API DECL_IMPORT
#endif

enum DAFS_OS
{
    DAFS_OSunknown,
    DAFS_OSwindows,
    DAFS_OSlinux,
    DAFS_OSsolaris
};

extern REMOTE_API void filenameToUrl(StringBuffer & out, const char * filename);

interface IDaFileSrvHook : extends IRemoteFileCreateHook
{
    virtual void addSubnetFilter(const char *subnet, const char *mask, const char *dir, const char *sourceRange, bool trace) = 0;
    virtual void addRangeFilter(const char *range, const char *dir, const char *sourceRange, bool trace) = 0;
    virtual IPropertyTree *addFilters(IPropertyTree *filters, const SocketEndpoint *ipAddress) = 0;
    virtual IPropertyTree *addMyFilters(IPropertyTree *filters, SocketEndpoint *myEp=NULL) = 0;
    virtual void clearFilters() = 0;
};
extern REMOTE_API IDaFileSrvHook *queryDaFileSrvHook();
extern REMOTE_API unsigned short getDaliServixPort();  // assumed just the one for now
extern REMOTE_API void setCanAccessDirectly(RemoteFilename & file,bool set);
extern REMOTE_API void setDaliServixSocketCaching(bool set);
extern REMOTE_API bool canAccessDirectly(const RemoteFilename & file);
extern REMOTE_API IFile *createDaliServixFile(const RemoteFilename & file);
extern REMOTE_API bool testDaliServixPresent(const IpAddress &ip);
extern REMOTE_API unsigned short getActiveDaliServixPort(const IpAddress &ip);
extern REMOTE_API unsigned getDaliServixVersion(const IpAddress &ip,StringBuffer &ver);
extern REMOTE_API unsigned getDaliServixVersion(const SocketEndpoint &ep,StringBuffer &ver);
extern REMOTE_API DAFS_OS getDaliServixOs(const SocketEndpoint &ep);

extern REMOTE_API void setLocalMountRedirect(const IpAddress &ip,const char *dir,const char *mountdir);
// redirects a daliservix file to a local mount. To remove redirect use NULL for mount dir or NULL for dir


extern REMOTE_API void remoteExtractBlobElements(const char * prefix, const RemoteFilename &file, ExtractedBlobArray & extracted);

extern REMOTE_API int setDafileSvrTraceFlags(const SocketEndpoint &ep,byte flags);
extern REMOTE_API int setDafileSvrThrottleLimit(const SocketEndpoint &_ep, ThrottleClass throttleClass, unsigned throttleLimit, unsigned throttleDelayMs, unsigned throttleCPULimit, unsigned queueLimit, StringBuffer *errMsg=NULL);
extern REMOTE_API int getDafileSvrInfo(const SocketEndpoint &ep, unsigned level, StringBuffer &retstr);

extern REMOTE_API void disconnectRemoteFile(IFile *file);
extern REMOTE_API void disconnectRemoteIoOnExit(IFileIO *fileio,bool set=true);

extern REMOTE_API bool resetRemoteFilename(IFile *file, const char *newname); // returns false if not remote


extern REMOTE_API void enableAuthentication(bool set=true); // default enabled for clients, disabled for server

extern REMOTE_API bool asyncCopyFileSection(const char *uuid,   // from genUUID - must be same for subsequent calls
                            IFile *from,                        // expected to be remote
                            RemoteFilename &to,
                            offset_t toofs,                     // (offset_t)-1 created file and copies to start
                            offset_t fromofs,
                            offset_t size,                      // (offset_t)-1 for all file
                            ICopyFileProgress *progress,
                            unsigned timeout                    // 0 to start, non-zero to wait
                        ); // returns true when done

extern REMOTE_API void setRemoteFileTimeouts(unsigned maxconnecttime,unsigned maxreadtime);

#define DAFS_VALIDATE_CONNECT_FAIL  (0x01)
#define DAFS_VALIDATE_BAD_VERSION   (0x02)
#define DAFS_VALIDATE_WRITE_FAIL_DATA  (0x12)
#define DAFS_VALIDATE_READ_FAIL_DATA   (0x14)
#define DAFS_VALIDATE_DISK_FULL_DATA   (0x18)
#define DAFS_VALIDATE_WRITE_FAIL_MIRROR  (0x22)
#define DAFS_VALIDATE_READ_FAIL_MIRROR   (0x24)
#define DAFS_VALIDATE_DISK_FULL_MIRROR   (0x28)
#define DAFS_SCRIPT_FAIL            (0x40)
                                
extern REMOTE_API unsigned validateNodes(const SocketEndpointArray &eps,const char *dataDir, const char *mirrorDir, bool chkver, SocketEndpointArray &failures, UnsignedArray &failedcodes, StringArray &failedmessages, const char *filename=NULL);

extern REMOTE_API void installFileHooks(const char *filespec);
extern REMOTE_API void removeFileHooks(); // Should be called before closedown

#endif
