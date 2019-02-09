#include "CQLogger.h"
#include "GlobalVar.h"
#include <map>
#include "RDConstant.h"

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
	{"strRuleNotFound", "δ�ҵ���Ӧ�Ĺ�����Ϣ"},
	{"strProp", "{0}��{1}����ֵΪ{2}"},
	{"strStErr", "��ʽ����:��ο������ĵ���ȡ.st�����ʹ�÷���"},
	{"strRulesFormatErr", "��ʽ����:��ȷ��ʽΪ.rules[��������:]������Ŀ ��.rules COC7:����"},
	{"strBanned", ">�����������أ�" },
	{"strGroupBanned", "���ﱻ�������أ�" },
	{"strNoAuth","�����㷢���˲��÷��ֵĶ����أ�"},
	{"strAlreadyBanned","�ѷ����" },
	{"strNotBanned","δ���" },
	{"strUnBan","�ѽ�����" },
	{"strSuccessfullyBanned","�Ѽ�����������" },
	{"strAutoBan","�Զ�ͬ�����������" },
	{"strAddDice","�ɹ��������Ӽ���ͬ���б�" },
	{"strDeleteDice","�ɹ����������Ƴ�ͬ���б�" },
	{"strAddGroup","�ɹ�����QQȺ����ͬ���б�" },
	{"strDeleteGroup","�ɹ�����QQȺ�Ƴ�ͬ���б�" },
	{"strBanAdmin","�벻Ҫ������ӹ���Ա" },
	{"strBanEmpty","����ôʲô��û����?"},
	{"strBanMaster","���ڳ��Ըɵ��Լ�ô��"},
	{"strFixTime","���ѣ����յĶ��ӽ���������ڿ�ʼ����Ϊ�������ӵ�ά��"},
	{"strSCP","SCP-"},
	{"strSCPWeb",R"(���ӵ�ַhttps://trow.cc/wiki/scp/main/database/series)"},
	{"strSCPErr",R"(��ʽ����:��ȷ��ʽΪ.scp [SCP����] ��.scp 173
�������The Ring of Wonder SCP Wiki
https://trow.cc/wiki/scp/start)"},
	{"strHlpMsgList",R"(<ͨ������>
r/rs [�������ʽ*] [ԭ��]	��ͨ����/�����
coc7/6 [����]	COC7/6��������
dnd [����]	DND��������
help	��ʾ����
<����Ⱥ/������>
ti/li	�����-��ʱ/�ܽ�֢״
st [del/clr/show] [������] [����ֵ]	���￨����
rc/ra [������] [����ֵ]	���ܼ춨(������/����)
w/ww XaY	����
set [1-99999֮�������]	����Ĭ����
sc SC���ʽ** [Sanֵ]	�Զ�Sancheck
en [������] [����ֵ]	��ǿ�춨/Ļ��ɳ�
ri [��ֵ] [�ǳ�]	DnD�ȹ�����
init [clr]	DnD�ȹ��鿴/���
nn [����]	����/ɾ���ǳ�
rh [�������ʽ*] [ԭ��]	����,���˽�ķ���
bot [on/off] [������QQ��]	�����˿�����ر�
ob [exit/list/clr/on/off]	�Թ�ģʽ
me [on/off/����]	�Ե������ӽ���������
welcome [��ӭ��Ϣ]	Ⱥ��ӭ��ʾ
fate [on/off]	��������
tarot	�����Ƴ鿨
<����˽��>
me [Ⱥ��] [����]	�Ե������ӽ���������
*COC7�ͷ���ΪP+����,������ΪB+����
 ֧��ʹ��K��ȡ�ϴ�ļ�������
 ֧��ʹ�� ����#���ʽ ���ж�������
**SC���ʽΪ �ɹ���San/ʧ�ܿ�San,��:1/1d6)"},
	{"strHlpMsg" , Dice_Full_Ver + "\n" +
	R"(<��ʹ�� .quit �����þ��յĶ�������!>
Ŀǰ��ʹ������(ǰ��"."��ʾ����):
r/rs|coc7/6|dnd|help|ti/li
st|rc/ra|w/ww|set|sc|en|ri
init|rh|botob|me|welcome
fate|tarot|scp
<����������ʹ��.help list�鿴>
<ʹ���������Ҿ��ս�(2558272027)>)"}
};