#include "BSODComponent.h"

#include "Debug.h"
#include "imgui.h"

#ifdef _WIN32
#include <iostream>
#include <Windows.h>
#include <winternl.h>
using namespace std;
typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
#endif

cs::BSODComponent::BSODComponent()
{
	type = ComponentMeta::BSOD_COMPONENT_TYPE;
}

void cs::BSODComponent::Init() {}

void cs::BSODComponent::Die()
{
#ifdef _WIN32
	BOOLEAN bEnabled;
	ULONG uResp;
	LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
	LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtRaiseHardError");
	pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
	pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
	NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
	NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
#else
	Debug::Log("I haven't implemented this button for anything but windows");
#endif
}

void cs::BSODComponent::ImGuiDrawComponent()
{
	if (ImGui::TreeNodeEx("BSOD", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("This component is meant as a joke but,\npressing the folded button\nwill actually give you a BSOD.");
		if (ImGui::TreeNode("Proceed"))
		{
			ImGui::Text("The BSOD will delete all unsaved work\nand possibly corrupt any ongoing processes.\nAre you sure you want to proceed?");
			if (ImGui::TreeNode("Proceed"))
			{
				ImGui::Text("Well... here it is");
				ImGui::Button("Die");
				if (ImGui::IsItemClicked())
				{
					Die();
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		
		ImGui::TreePop();
	}
}
