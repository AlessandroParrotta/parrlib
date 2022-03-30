#include "WinsockUtil.h"

namespace prb {
	namespace WinsockUtil {
		std::string errorToString(int err) {
			switch (err) {
			case 6:

				return "Specified event object handle is invalid.";
				break;
			case 8:

				return "Insufficient memory available.";
				break;
			case 87:

				return "One or more parameters are invalid.";
				break;
			case 995:

				return "Overlapped operation aborted.";
				break;
			case 996:

				return "Overlapped I/O event object not in signaled state.";
				break;
			case 997:

				return "Overlapped operations will complete later.";
				break;
			case 10004:

				return "Interrupted function call.";
				break;
			case 10009:

				return "File handle is not valid.";
				break;
			case 10013:

				return "Permission denied.";
				break;
			case 10014:

				return "Bad address.";
				break;
			case 10022:

				return "Invalid argument.";
				break;
			case 10024:

				return "Too many open files.";
				break;
			case 10035:

				return "Resource temporarily unavailable.";
				break;
			case 10036:

				return "Operation now in progress.";
				break;
			case 10037:

				return "Operation already in progress.";
				break;
			case 10038:

				return "Socket operation on nonsocket.";
				break;
			case 10039:

				return "Destination address required.";
				break;
			case 10040:

				return "Message too long.";
				break;
			case 10041:

				return "Protocol wrong type for socket.";
				break;
			case 10042:

				return "Bad protocol option.";
				break;
			case 10043:

				return "Protocol not supported.";
				break;
			case 10044:

				return "Socket type not supported.";
				break;
			case 10045:

				return "Operation not supported.";
				break;
			case 10046:

				return "Protocol family not supported.";
				break;
			case 10047:

				return "Address family not supported by protocol family.";
				break;
			case 10048:

				return "Address already in use.";
				break;
			case 10049:

				return "Cannot assign requested address.";
				break;
			case 10050:

				return "Network is down.";
				break;
			case 10051:

				return "Network is unreachable.";
				break;
			case 10052:

				return "Network dropped connection on reset.";
				break;
			case 10053:

				return "Software caused connection abort.";
				break;
			case 10054:

				return "Connection reset by peer.";
				break;
			case 10055:

				return "No buffer space available.";
				break;
			case 10056:

				return "Socket is already connected.";
				break;
			case 10057:

				return "Socket is not connected.";
				break;
			case 10058:

				return "Cannot send after socket shutdown.";
				break;
			case 10059:

				return "Too many references.";
				break;
			case 10060:

				return "Connection timed out.";
				break;
			case 10061:

				return "Connection refused.";
				break;
			case 10062:

				return "Cannot translate name.";
				break;
			case 10063:

				return "Name too long.";
				break;
			case 10064:

				return "Host is down.";
				break;
			case 10065:

				return "No route to host.";
				break;
			case 10066:

				return "Directory not empty.";
				break;
			case 10067:

				return "Too many processes.";
				break;
			case 10068:

				return "User quota exceeded.";
				break;
			case 10069:

				return "Disk quota exceeded.";
				break;
			case 10070:

				return "Stale file handle reference.";
				break;
			case 10071:

				return "Item is remote.";
				break;
			case 10091:

				return "Network subsystem is unavailable.";
				break;
			case 10092:

				return "Winsock.dll version out of range.";
				break;
			case 10093:

				return "Successful WSAStartup not yet performed.";
				break;
			case 10101:

				return "Graceful shutdown in progress.";
				break;
			case 10102:

				return "No more results.";
				break;
			case 10103:

				return "Call has been canceled.";
				break;
			case 10104:

				return "Procedure call table is invalid.";
				break;
			case 10105:

				return "Service provider is invalid.";
				break;
			case 10106:

				return "Service provider failed to initialize.";
				break;
			case 10107:

				return "System call failure.";
				break;
			case 10108:

				return "Service not found.";
				break;
			case 10109:

				return "Class type not found.";
				break;
			case 10110:

				return "No more results.";
				break;
			case 10111:

				return "Call was canceled.";
				break;
			case 10112:

				return "Database query was refused.";
				break;
			case 11001:

				return "Host not found.";
				break;
			case 11002:

				return "Nonauthoritative host not found.";
				break;
			case 11003:

				return "This is a nonrecoverable error."
					; break;
			case 11004:

				return "Valid name, no data record of requested type.";
				break;
			case 11005:

				return "QoS receivers.";
				break;
			case 11006:

				return "QoS senders.";
				break;
			case 11007:

				return "No QoS senders.";
				break;
			case 11008:

				return "QoS no receivers.";
				break;
			case 11009:

				return "QoS request confirmed.";
				break;
			case 11010:

				return "QoS admission error.";
				break;
			case 11011:

				return "QoS policy failure.";
				break;
			case 11012:

				return "QoS bad style.";
				break;
			case 11013:

				return "QoS bad object.";
				break;
			case 11014:

				return "QoS traffic control error.";
				break;
			case 11015:

				return "QoS generic error.";
				break;
			case 11016:

				return "QoS service type error.";
				break;
			case 11017:

				return "QoS flowspec error.";
				break;
			case 11018:

				return "Invalid QoS provider buffer.";
				break;
			case 11019:

				return "Invalid QoS filter style.";
				break;
			case 11020:

				return "Invalid QoS filter type.";
				break;
			case 11021:

				return "Incorrect QoS filter count.";
				break;
			case 11022:

				return "Invalid QoS object length.";
				break;
			case 11023:

				return "Incorrect QoS flow count.";
				break;
			case 11024:

				return "Unrecognized QoS object.";
				break;
			case 11025:

				return "Invalid QoS policy object.";
				break;
			case 11026:

				return "Invalid QoS flow descriptor.";
				break;
			case 11027:

				return "Invalid QoS provider-specific flowspec.";
				break;
			case 11028:

				return "Invalid QoS provider-specific filterspec.";
				break;
			case 11029:

				return "Invalid QoS shape discard mode object.";
				break;
			case 11030:

				return "Invalid QoS shaping rate object.";
				break;
			case 11031:

				return "Reserved policy QoS element type.";
				break;
			default:
				return "Unknown error.";
			}
		}
	}
}
