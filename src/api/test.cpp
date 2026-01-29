// #include <stdio.h>
// #include <Windows.h>
// #include <Oleacc.h>
// #pragma comment( lib,"Oleacc.lib")

// HWINEVENTHOOK LHook = 0;

// void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
//   IAccessible* pAcc = NULL;
//   VARIANT varChild;
//   if ((AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &varChild) == S_OK) && (pAcc != NULL)) {
//     char className[50];
//     if (GetClassName(hwnd, className, 50) && strcmp(className, "Chrome_WidgetWin_1") == 0) {
//       BSTR bstrName = nullptr;
//       if (pAcc->get_accName(varChild, &bstrName) == S_OK) {
//         if (wcscmp(bstrName, L"Address and search bar") == 0) {
//           BSTR bstrValue = nullptr;
//           if (pAcc->get_accValue(varChild, &bstrValue) == S_OK) {
//             printf("URL change: %ls\n", bstrValue);
//             SysFreeString(bstrValue);
//           }
//         }
//         SysFreeString(bstrName);
//       }
//       pAcc->Release();
//     }
//   }
// }

// void Hook() {
//     if (LHook != 0) return;
//     CoInitialize(NULL);
//     LHook = SetWinEventHook(EVENT_OBJECT_FOCUS, EVENT_OBJECT_VALUECHANGE, 0, WinEventProc, 0, 0, WINEVENT_SKIPOWNPROCESS);
// }

// void Unhook() {
//     if (LHook == 0) return;
//     UnhookWinEvent(LHook);
//     CoUninitialize();
// }


// int main(int argc, const char* argv[]) {
//     MSG msg;
//     Hook();

//     while (GetMessage(&msg, NULL, 0, 0)) {
//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     }

//     Unhook();

//     return 0;
// }