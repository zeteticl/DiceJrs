#include "CQLogger.h"
#include "GlobalVar.h"
#include <map>
#include "RDConstant.h"
#include "Jrs.h"

bool Enabled = false;

bool msgSendThreadRunning = false;

CQ::logger DiceLogger("DiceJrs");

std::map<std::string, std::string> GlobalMsg
{
	{"strSetInvalid", "��Ч��Ĭ����!������1-99999֮�������!"},
	{"strSetTooBig", "Ĭ��������!������1-99999֮�������!"},
	{"strSetCannotBeZero", "Ĭ��������Ϊ��!������1-99999֮�������!"},
	{"strCharacterCannotBeZero", "�������ɴ�������Ϊ��!������1-10֮�������!"},
	{"strSetInvalid", "��Ч��Ĭ����!������1-99999֮�������!"},
	{"strSetTooBig", "Ĭ��������!������1-99999֮�������!"},
	{"strSetCannotBeZero", "Ĭ��������Ϊ��!������1-99999֮�������!"},
	{"strCharacterCannotBeZero", "�������ɴ�������Ϊ��!������1-10֮�������!"},
	{"strCharacterTooBig", "�������ɴ�������!������1-10֮�������!"},
	{"strCharacterInvalid", "�������ɴ�����Ч!������1-10֮�������!"},
	{"strSCInvalid", "SC���ʽ���벻��ȷ,��ʽΪ�ɹ���San/ʧ�ܿ�San,��1/1d6!"},
	{"strSanInvalid", "Sanֵ���벻��ȷ,������1-99��Χ�ڵ�����!"},
	{"strEnValInvalid", "����ֵ���������벻��ȷ,������1-99��Χ�ڵ�����!"},
	{"strGroupIDInvalid", "��Ч��Ⱥ��!"},
	{"strSendErr", "��Ϣ����ʧ��!"},
	{"strDisabledErr", "���յĶ������ڴ�Ⱥ�б��ر�!"},
	{"strMEDisabledErr", "����Ա���ڴ�Ⱥ�н���.me����!"},
	{"strHELPDisabledErr", "����Ա���ڴ�Ⱥ�н���.help����!"},
	{"strNameDelErr", "û����������,�޷�ɾ��!"},
	{"strValueErr", "�������ʽ�������!"},
	{"strInputErr", "������������ʽ�������!"},
	{"strUnknownErr", "������δ֪����!"},
	{"strDiceTooBigErr", "���յĶ��ӱ����ӳ���������û��"},
	{"strTypeTooBigErr", "��!�������������ж�������~1...2..."},
	{"strZeroTypeErr", "����...!!ʱ����(���յĶ��ӱ����Ӳ�����ʱ���ѷ������)"},
	{"strAddDiceValErr", "������Ҫ�����������ӵ�ʲôʱ����~(��������ȷ�ļ�������:5-10֮�ڵ�����)"},
	{"strZeroDiceErr", "��?�ҵ�������?"},
	{"strRollTimeExceeded", "�������������������������!"},
	{"strRollTimeErr", "�쳣����������"},
	{"strWelcomeMsgClearNotice", "�������Ⱥ����Ⱥ��ӭ��"},
	{"strWelcomeMsgClearErr", "����:û��������Ⱥ��ӭ�ʣ����ʧ��"},
	{"strWelcomeMsgUpdateNotice", "�Ѹ��±�Ⱥ����Ⱥ��ӭ��"},
	{"strPermissionDeniedErr", "����:�˲�����ҪȺ�������ԱȨ��"},
	{"strNameTooLongErr", "����:���ƹ���(���Ϊ50Ӣ���ַ�)"},
	{"strUnknownPropErr", "����:���Բ�����"},
	{"strEmptyWWDiceErr", "��ʽ����:��ȷ��ʽΪ.w(w)XaY!����X��1, 5��Y��10"},
	{"strPropErr", "������������������Ŷ~"},
	{"strSetPropSuccess", "�������óɹ�"},
	{"strPropCleared", "�������������"},
	{"strPropDeleted", "����ɾ���ɹ�"},
	{"strPropNotFound", "����:���Բ�����"},
	{"strRuleErr", "�������ݻ�ȡʧ��,������Ϣ:\n"},
	{"strRulesFailedErr", "����ʧ��,�޷��������ݿ�"},
	{"strRuleNotFound", "δ�ҵ���Ӧ�Ĺ�����Ϣ"},
	{"strProp", "{0}��{1}����ֵΪ{2}"},
	{"strStErr", "��ʽ����:��ο������ĵ���ȡ.st�����ʹ�÷���"},
	{"strBanned", ">�����������أ�" },
	{"strGroupBanned", "���ﱻ�������أ�" },
	{"strNoAuth","�����㷢���˲��÷��ֵĶ����أ�"},
	{"strAlreadyBanned","�ѷ����" },
	{"strNotBanned","δ���" },
	{"strUnBan","�ѽ�����" },
	{"strJrrp", "{0}�������Ʒֵ��: {1}"},
	{"strJrrpErr", "JRRP��ȡʧ��! ������Ϣ: \n{0}"},
	{"strSuccessfullyBanned","�Ѽ�����������" },
	{"strBanAdmin","�벻Ҫ������ӹ���Ա" },
	{"strBanEmpty","����ôʲô��û����?"},
	{"strBanMaster","���ڳ��Ըɵ��Լ�ô��"},
	{"strFixTime","���ѣ����յĶ��ӽ���������ڿ�ʼ����Ϊ�������ӵ�ά��"},
	{"strSCP","SCP-"},
	{"strSCPWeb",R"(���ӵ�ַhttps://trow.cc/wiki/scp/main/database/series)"},
	{"strSCPErr",R"(��ʽ����:��ȷ��ʽΪ.scp [SCP����] ��.scp 173
�������The Ring of Wonder SCP Wiki
https://trow.cc/wiki/scp/start)"},
};