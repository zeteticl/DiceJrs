#include "CQLogger.h"
#include "GlobalVar.h"
#include <map>
#include "RDConstant.h"

bool Enabled = false;

bool msgSendThreadRunning = false;

CQ::logger DiceLogger("DiceJrs");

std::map<std::string, std::string> GlobalMsg
{
	{"strSetInvalid", "无效的默认骰!请输入1-99999之间的数字!"},
	{"strSetTooBig", "默认骰过大!请输入1-99999之间的数字!"},
	{"strSetCannotBeZero", "默认骰不能为零!请输入1-99999之间的数字!"},
	{"strCharacterCannotBeZero", "人物作成次数不能为零!请输入1-10之间的数字!"},
	{"strSetInvalid", "无效的默认骰!请输入1-99999之间的数字!"},
	{"strSetTooBig", "默认骰过大!请输入1-99999之间的数字!"},
	{"strSetCannotBeZero", "默认骰不能为零!请输入1-99999之间的数字!"},
	{"strCharacterCannotBeZero", "人物作成次数不能为零!请输入1-10之间的数字!"},
	{"strCharacterTooBig", "人物作成次数过多!请输入1-10之间的数字!"},
	{"strCharacterInvalid", "人物作成次数无效!请输入1-10之间的数字!"},
	{"strSCInvalid", "SC表达式输入不正确,格式为成功扣San/失败扣San,如1/1d6!"},
	{"strSanInvalid", "San值输入不正确,请输入1-99范围内的整数!"},
	{"strEnValInvalid", "技能值或属性输入不正确,请输入1-99范围内的整数!"},
	{"strGroupIDInvalid", "无效的群号!"},
	{"strSendErr", "消息发送失败!"},
	{"strDisabledErr", "旧日的儿子已在此群中被关闭!"},
	{"strMEDisabledErr", "管理员已在此群中禁用.me命令!"},
	{"strHELPDisabledErr", "管理员已在此群中禁用.help命令!"},
	{"strNameDelErr", "没有设置名称,无法删除!"},
	{"strValueErr", "掷骰表达式输入错误!"},
	{"strInputErr", "命令或掷骰表达式输入错误!"},
	{"strUnknownErr", "发生了未知错误!"},
	{"strDiceTooBigErr", "旧日的儿子被你扔出的骰子淹没了"},
	{"strTypeTooBigErr", "哇!让我数数骰子有多少面先~1...2..."},
	{"strZeroTypeErr", "这是...!!时空裂(旧日的儿子被骰子产生的时空裂缝卷走了)"},
	{"strAddDiceValErr", "你这样要让我扔骰子扔到什么时候嘛~(请输入正确的加骰参数:5-10之内的整数)"},
	{"strZeroDiceErr", "咦?我的骰子呢?"},
	{"strRollTimeExceeded", "掷骰轮数超过了最大轮数限制!"},
	{"strRollTimeErr", "异常的掷骰轮数"},
	{"strWelcomeMsgClearNotice", "已清除本群的入群欢迎词"},
	{"strWelcomeMsgClearErr", "错误:没有设置入群欢迎词，清除失败"},
	{"strWelcomeMsgUpdateNotice", "已更新本群的入群欢迎词"},
	{"strPermissionDeniedErr", "错误:此操作需要群主或管理员权限"},
	{"strNameTooLongErr", "错误:名称过长(最多为50英文字符)"},
	{"strUnknownPropErr", "错误:属性不存在"},
	{"strEmptyWWDiceErr", "格式错误:正确格式为.w(w)XaY!其中X≥1, 5≤Y≤10"},
	{"strPropErr", "请认真的输入你的属性哦~"},
	{"strSetPropSuccess", "属性设置成功"},
	{"strPropCleared", "已清除所有属性"},
	{"strPropDeleted", "属性删除成功"},
	{"strPropNotFound", "错误:属性不存在"},
	{"strRuleNotFound", "未找到对应的规则信息"},
	{"strProp", "{0}的{1}属性值为{2}"},
	{"strStErr", "格式错误:请参考帮助文档获取.st命令的使用方法"},
	{"strRulesFormatErr", "格式错误:正确格式为.rules[规则名称:]规则条目 如.rules COC7:力量"},
	{"strBanned", ">您被划掉了呢！" },
	{"strGroupBanned", "这里被划掉了呢！" },
	{"strNoAuth","看来你发现了不该发现的东西呢！"},
	{"strAlreadyBanned","已封禁！" },
	{"strNotBanned","未封禁" },
	{"strUnBan","已解除封禁" },
	{"strSuccessfullyBanned","已加入封禁名单！" },
	{"strAutoBan","自动同步封禁名单！" },
	{"strAddDice","成功将该骰子加入同步列表" },
	{"strDeleteDice","成功将该骰子移出同步列表" },
	{"strAddGroup","成功将该QQ群加入同步列表" },
	{"strDeleteGroup","成功将该QQ群移出同步列表" },
	{"strBanAdmin","请不要封禁骰子管理员" },
	{"strBanEmpty","我怎么什么都没看到?"},
	{"strBanMaster","您在尝试干掉自己么？"},
	{"strFixTime","提醒，旧日的儿子将于五分钟内开始进行为期两分钟的维护"},
	{"strSCP","SCP-"},
	{"strSCPWeb",R"(链接地址https://trow.cc/wiki/scp/main/database/series)"},
	{"strSCPErr",R"(格式错误:正确格式为.scp [SCP号码] 如.scp 173
服务基于The Ring of Wonder SCP Wiki
https://trow.cc/wiki/scp/start)"},
	{"strHlpMsgList",R"(<通用命令>
r/rs [掷骰表达式*] [原因]	普通掷骰/简化输出
coc7/6 [个数]	COC7/6人物作成
dnd [个数]	DND人物作成
help	显示帮助
<仅限群/讨论组>
ti/li	疯狂发作-临时/总结症状
st [del/clr/show] [属性名] [属性值]	人物卡导入
rc/ra [技能名] [技能值]	技能检定(规则书/房规)
w/ww XaY	骰池
set [1-99999之间的整数]	设置默认骰
sc SC表达式** [San值]	自动Sancheck
en [技能名] [技能值]	增强检定/幕间成长
ri [加值] [昵称]	DnD先攻掷骰
init [clr]	DnD先攻查看/清空
nn [名称]	设置/删除昵称
rh [掷骰表达式*] [原因]	暗骰,结果私聊发送
bot [on/off] [机器人QQ号]	机器人开启或关闭
ob [exit/list/clr/on/off]	旁观模式
me [on/off/动作]	以第三方视角做出动作
welcome [欢迎消息]	群欢迎提示
fate [on/off]	今日命运
tarot	塔罗牌抽卡
<仅限私聊>
me [群号] [动作]	以第三方视角做出动作
*COC7惩罚骰为P+个数,奖励骰为B+个数
 支持使用K来取较大的几个骰子
 支持使用 个数#表达式 进行多轮掷骰
**SC表达式为 成功扣San/失败扣San,如:1/1d6)"},
	{"strHlpMsg" , Dice_Full_Ver + "\n" +
	R"(<请使用 .quit 命令让旧日的儿子脱离!>
目前可使用命令(前加"."已示命令):
r/rs|coc7/6|dnd|help|ti/li
st|rc/ra|w/ww|set|sc|en|ri
init|rh|botob|me|welcome
fate|tarot|scp
<命令详情请使用.help list查看>
<使用问题请找旧日酱(2558272027)>)"}
};