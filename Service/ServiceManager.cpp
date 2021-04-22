#include "ServiceManager.h"
namespace ServiceLib::ServiceManagement
{

	int ServiceManager::InstallService()
	{
		SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		if (!hSCManager) {
			
			return -1;
		}
		LPWSTR serviceName = reinterpret_cast<LPWSTR>(const_cast<char16_t*>(m_serviceName.c_str()));
		LPWSTR servicePath = reinterpret_cast<LPWSTR>(const_cast<char16_t*>(m_servicePath.c_str()));

		SC_HANDLE hService = CreateServiceW(
			hSCManager,
			serviceName,
			serviceName,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,
			servicePath,
			NULL, NULL, NULL, NULL, NULL
		);

		if (!hService) {
			
			CloseServiceHandle(hSCManager);
			return -1;
		}
		CloseServiceHandle(hService);

		CloseServiceHandle(hSCManager);
		
		return 0;
	}

	int ServiceManager::UninstallService()
	{
		SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (!hSCManager) {
			
			return -1;
		}
		SC_HANDLE hService = OpenServiceW(
			hSCManager,
			reinterpret_cast<LPWSTR>(const_cast<char16_t*>(m_serviceName.c_str())),
			SERVICE_STOP | DELETE
		);

		if (!hService) {
			
			CloseServiceHandle(hSCManager);
			return -1;
		}

		DeleteService(hService);
		CloseServiceHandle(hService);
		
		
		return 0;
	}

	int ServiceManager::RunService()
	{
		SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		SC_HANDLE hService = OpenService(
			hSCManager,
			reinterpret_cast<LPWSTR>(const_cast<char16_t*>(m_serviceName.c_str())),
			SERVICE_START
		);

		if (!StartService(hService, 0, NULL)) {
			CloseServiceHandle(hSCManager);
			
			return -1;
		}

		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return 0;
	}
}