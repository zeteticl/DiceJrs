#pragma once
#ifndef Jrs_Msg
#define Jrs_Msg
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h> 

constexpr long long MASTER = 2558272027;
constexpr int strFix = 0;

//Version
const unsigned short Dice_Build = 509;
const std::string Dice_Ver_Without_Build = "2.3.6";
const std::string DiceRequestHeader = "Dice/" + Dice_Ver_Without_Build;
const std::string Dice_Ver = Dice_Ver_Without_Build + "(" + std::to_string(Dice_Build) + ")";
const std::string Dice_Short_Ver = "By 旧日酱 Base on Dice 2.3.6" ;
const std::string Dice_Head_Ver = "<" + Dice_Short_Ver + ">";
const std::string Dice_Full_Ver = "旧日的儿子 DiceJRS\n" + Dice_Head_Ver;
const std::string Dice_Bot_Ver = "我，是旧日的儿子！有何贵干？";

//Help
const std::string strHlpMsg = Dice_Full_Ver + R"(
<请使用 .quit 命令让旧日的儿子脱离!>
目前可使用命令(前加"."已示命令):
r/rs|coc7/6|dnd|help|ti/li
st|rc/ra|w/ww|set|sc|en|ri
init|rh|botob|me|welcome
fate|tarot|jrrp
<命令详情请使用.help list查看>
<使用问题请找旧日酱(2558272027)>)";

const std::string strHlpMsgList = R"(<通用命令>
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
jrrp	每日人品
tarot	塔罗牌抽卡
<仅限私聊>
me [群号] [动作]	以第三方视角做出动作
------备注------
*COC7惩罚骰为P+个数,奖励骰为B+个数
 支持使用K来取较大的几个骰子
 支持使用 个数#表达式 进行多轮掷骰
**SC表达式为 成功扣San/失败扣San,如:1/1d6)";

//Fate
typedef struct FATE
{
	int FATEVal;
	std::string Date;
}FATE;

//卡牌
static std::string tarotCard[] =
{
	"",
	"[愚者 正位]\n无忧无虑、轻松愉快。古怪、与众不同。不成熟。出乎意料的。",
	"[愚者 逆位]\n奇特的习惯。精神错乱、胡言乱语。疯狂、发怒。狂喜、陶醉。",
	"[魔法师 正位]\n自愿、乐意。自治、独立。技术、熟练。精明、灵敏。外交、交际。",
	"[魔法师 逆位]\n说谎。攀权附贵。诡计、谋略、耍花招。",
	"[女祭司 正位]\n学习、研究。直觉。秘密。信念、信赖。神秘。",
	"[女祭司 逆位]\n无知。偏见。歇斯底里。",
	"[女帝 正位]\n肥沃、多产。理解。交换意见。帮助、分享。怀孕。",
	"[女帝 逆位]\n愚蠢。不孕。轻薄、无趣。",
	"[皇帝 正位]\n稳定、安定。力量。权力。守护、保卫。",
	"[皇帝 逆位]\n注视着伟大的事物。傲慢、自大。敌对、反抗。",
	"[法皇 正位]\n灵感。慈悲、仁爱。慰藉、援助、解救。有耐心。依循传统。",
	"[法皇 逆位]\n仇恨、憎恨。无法宽容。不道德、邪恶。",
	"[恋人 正位]\n选择。企图、尝试。检查、调查。缔结、婚约、订约。合并、结合、合而为一。",
	"[恋人 逆位]\n不守誓约、通奸、外遇。分离。优柔寡断。",
	"[战车 正位]\n胜利。经营的技能。公众人物、大众认可。",
	"[战车 逆位]\n失败。无能。失误。",
	"[正义 正位]\n冷静、明智。法律。逻辑、有道理的。划分阶级。",
	"[正义 逆位]\n不公平、失去正义。法律问题。混乱、没有秩序。",
	"[隐者 正位]\n谨慎、慎重。沉思、冥想。孤独、隐居。寂静、无声。",
	"[隐者 逆位]\n顾影自怜。妒忌。拖延、延迟、耽搁。",
	"[命运之轮 正位]\n轮流、交替。大自然的循环。改变。好机会。",
	"[命运之轮 逆位]\n不稳定。失去优势。",
	"[力量 正位]\n活力、能量。道德勇气。棘手的工作、强健的工作。勇敢。",
	"[力量 逆位]\n怠惰、懒散。冲动。虚弱、软弱。",
	"[吊人 正位]\n牺牲、奉献。理想化、理想主义。有利他人的行为、利他主义。对神秘事物的狂热份子。",
	"[吊人 逆位]\n无能。生病。",
	"[死神 正位]\n结束。突然的且激烈的改变。开始。",
	"[死神 逆位]\n危险的阻碍、令人担心的难关。逆境。",
	"[节制 正位]\n温和、适度。适应、通融。足够的休息、有益健康的休养。关心。",
	"[节制 逆位]\n焦虑、挂念。不舒服、不愿意。",
	"[恶魔 正位]\n本能、天性。魅力、吸引力。感官享受、好色。暗示。",
	"[恶魔 逆位]\n堕落、曲解。不安、烦乱。憎恨、讨厌。",
	"[高塔 正位]\n逃避、避免。仓促的起程。流放、流亡。粉碎必然的事物、打破习惯传统。危险。",
	"[高塔 逆位]\n事故、灾难。毁灭。混乱。",
	"[星 正位]\n希望。明显的指引。新的想法。和平、和睦。",
	"[星 逆位]\n不幸的征召。辞职、放弃。",
	"[月 正位]\n梦。幻觉。冒险。不可思议的遭遇。旅行。",
	"[月 逆位]\n危险。邪恶。谎言。",
	"[太阳 正位]\n同意、一致。友善、友谊。爱。荣誉。欢乐。",
	"[太阳 逆位]\n痛苦、不幸。利己主义、自我中心。易怒。",
	"[审判 正位]\n复活、回复、更新。生日、出生。觉醒。重新获得、恢复。",
	"[审判 逆位]\n怀疑、不相信。悔恨、懊悔、遗憾。生病。",
	"[世界 正位]\n报答、奖赏、结果。完美。成功。继承、遗传、传统。时间、一段时光。",
	"[世界 逆位]\n延期、拖延。迷惑、欺骗。失败。",
};
#endif /*Jrs_Msg*/