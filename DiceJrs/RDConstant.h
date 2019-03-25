﻿#pragma once
#ifndef JRS_RD_CONSTANT
#define JRS_RD_CONSTANT
#include <string>

//Error Handle
#define Value_Err (-1)
#define Input_Err (-2)
#define ZeroDice_Err (-3)
#define ZeroType_Err (-4)
#define DiceTooBig_Err (-5)
#define TypeTooBig_Err (-6)
#define AddDiceVal_Err (-7)
//Dice Type
#define Normal_Dice 0
#define B_Dice 1
#define P_Dice 2
#define Fudge_Dice 3
#define WW_Dice 4
//Message Type
#define PrivateMsg 0
#define GroupMsg 1
#define DiscussMsg 2
//Source type
#define PrivateT 0
#define GroupT 1
#define DiscussT 2

typedef int int_errno;

//属性
static std::map<std::string, std::string> SkillNameReplace = { 
	{"str", "力量"},
	{"dex", "敏捷"},
	{"pow", "意志"},
	{"siz", "体型"},
	{"app", "外貌"},
	{"luck", "幸运"},
	{"luk", "幸运"},
	{"con", "体质"},
	{"int", "智力/灵感"},
	{"智力", "智力/灵感"},
	{"灵感", "智力/灵感"},
	{"idea", "智力/灵感"},
	{"edu", "教育"},
	{"mov", "移动力"},
	{"san", "理智"},
	{"hp", "体力"},
	{"mp", "魔法"},
	{"侦察", "侦查"},
	{"计算机", "计算机使用"},
	{"电脑", "计算机使用"},
	{"电脑使用", "计算机使用"},
	{"信誉", "信用评级"},
	{"信誉度", "信用评级"},
	{"信用度", "信用评级"},
	{"信用", "信用评级"},
	{"驾驶", "汽车驾驶"},
	{"驾驶汽车", "汽车驾驶"},
	{"驾驶(汽车)", "汽车驾驶"},
	{"驾驶:汽车", "汽车驾驶"},
	{"快速交谈", "话术"},
	{"步枪", "步枪/霰弹枪"},
	{"霰弹枪", "步枪/霰弹枪"},
	{"散弹枪", "步枪/霰弹枪"},
	{"步霰", "步枪/霰弹枪"},
	{"步/霰", "步枪/霰弹枪"},
	{"步散", "步枪/霰弹枪"},
	{"步/散", "步枪/霰弹枪"},
	{"图书馆", "图书馆使用"},
	{"机修", "机械维修"},
	{"电器维修", "电气维修"},
	{"cm", "克苏鲁神话"},
	{"克苏鲁", "克苏鲁神话"},
	{"唱歌", "歌唱"},
	{"做画", "作画"},
	{"耕做", "耕作"},
	{"机枪", "机关枪"},
	{"导航", "领航"},
	{"船", "船驾驶"},
	{"驾驶船", "船驾驶"},
	{"驾驶(船)", "船驾驶"},
	{"驾驶:船", "船驾驶"},
	{"飞行器", "飞行器驾驶"},
	{"驾驶飞行器", "飞行器驾驶"},
	{"驾驶:飞行器", "飞行器驾驶"},
	{"驾驶(飞行器)", "飞行器驾驶"}
};

//技能
static std::map<std::string, int> SkillDefaultVal = {
	{"会计", 5},
	{"人类学", 1},
	{"估价", 5},
	{"考古学", 1},
	{"作画", 5},
	{"摄影", 5},
	{"表演", 5},
	{"伪造", 5},
	{"文学", 5},
	{"书法", 5},
	{"乐理", 5},
	{"厨艺", 5},
	{"裁缝", 5},
	{"理发", 5},
	{"建筑", 5},
	{"舞蹈", 5},
	{"酿酒", 5},
	{"捕鱼", 5},
	{"歌唱", 5},
	{"制陶", 5},
	{"雕塑", 5},
	{"杂技", 5},
	{"风水", 5},
	{"技术制图", 5},
	{"耕作", 5},
	{"打字", 5},
	{"速记", 5},
	{"魅惑", 15},
	{"攀爬", 20},
	{"计算机使用", 5},
	{"克苏鲁神话", 0},
	{"乔装", 5},
	{"汽车驾驶", 20},
	{"电气维修", 10},
	{"电子学", 1},
	{"话术", 5},
	{"鞭子", 5},
	{"电锯", 10},
	{"斧", 15},
	{"剑", 20},
	{"绞具", 25},
	{"链枷", 25},
	{"矛", 25},
	{"手枪", 20},
	{"步枪/霰弹枪", 25},
	{"冲锋枪", 15},
	{"弓术", 15},
	{"火焰喷射器", 10},
	{"机关枪", 10},
	{"重武器", 10},
	{"急救", 30},
	{"历史", 5},
	{"恐吓", 15},
	{"跳跃", 20},
	{"法律", 5},
	{"图书馆使用", 20},
	{"聆听", 20},
	{"锁匠", 1},
	{"机械维修", 10},
	{"医学", 1},
	{"自然学", 10},
	{"领航", 10},
	{"神秘学", 5},
	{"操作重型机械", 1},
	{"说服", 10},
	{"飞行器驾驶", 1},
	{"船驾驶", 1},
	{"精神分析", 1},
	{"心理学", 10},
	{"骑乘", 5},
	{"地质学", 1},
	{"化学", 1},
	{"生物学", 1},
	{"数学", 1},
	{"天文学", 1},
	{"物理学", 1},
	{"药学", 1},
	{"植物学", 1},
	{"动物学", 1},
	{"密码学", 1},
	{"工程学", 1},
	{"气象学", 1},
	{"司法科学", 1},
	{"妙手", 10},
	{"侦查", 25},
	{"潜行", 20},
	{"游泳", 20},
	{"投掷", 20},
	{"追踪", 10},
	{"驯兽", 5},
	{"潜水", 1},
	{"爆破", 1},
	{"读唇", 1},
	{"催眠", 1},
	{"炮术", 1},
	{"斗殴", 25}
};

//临时疯狂
static std::string TempInsanity[11]{
	"",
	"[失忆]\n在{0}轮之内，调查员会发现自己只记得最后身处的安全地点，却没有任何来到这里的记忆。",
	"[假性残疾]\n调查员陷入了心理性的失明，失聪以及躯体缺失感中，持续{0}轮。",
	"[暴力倾向]\n调查员陷入了六亲不认的暴力行为中，对周围的敌人与友方进行着无差别的攻击，持续{0}轮。",
	"[偏执]\n调查员陷入了严重的偏执妄想之中。有人在暗中窥视着他们，同伴中有人背叛了他们，没有人可以信任，万事皆虚。持续{0}轮。",
	"[人际依赖]\n调查员因为一些原因而将他人误认为了他重要的人并且努力的会与那个人保持那种关系，持续{0}轮。",
	"[昏厥]\n调查员当场昏倒，昏迷状态持续{0}轮。",
	"[逃避行为]\n调查员会用任何的手段试图逃离现在所处的位置，状态持续{0}轮。",
	"[竭嘶底里]\n调查员表现出大笑，哭泣，嘶吼，害怕等的极端情绪表现，持续{0}轮。",
	"[恐惧]\n调查员陷入对某一事物的恐惧之中，就算这一恐惧的事物是并不存在的，持续{0}轮。\n{1}\n具体恐惧症: {2}(守秘人也可以自行从恐惧症状表中选择其他症状)",
	"[躁狂]\n调查员由于某种诱因进入躁狂状态，症状持续{0}轮。\n{1}\n具体躁狂症: {2}(守秘人也可以自行从躁狂症状表中选择其他症状)"
};

//症状总结
static std::string LongInsanity[11]{
	"",
	"[失忆]\n在{0}小时后，调查员回过神来，发现自己身处一个陌生的地方，并忘记了自己是谁。记忆会随时间恢复。",
	"[被窃]\n调查员在{0}小时后恢复清醒，发觉自己被盗，身体毫发无损。",
	"[遍体鳞伤]\n调查员在{0}小时后恢复清醒，发现自己身上满是拳痕和瘀伤。生命值减少到疯狂前的一半，但这不会造成重伤。调查员没有被窃。这种伤害如何持续到现在由守秘人决定。",
	"[暴力倾向]\n调查员陷入强烈的暴力与破坏欲之中，持续{0}小时。",
	"[极端信念]\n在{0}小时之内，调查员会采取极端和疯狂的表现手段展示他们的思想信念之一",
	"[重要之人]\n在{0}小时中，调查员将不顾一切地接近重要的那个人，并为他们之间的关系做出行动。",
	"[被收容]\n{0}小时后，调查员在精神病院病房或警察局牢房中回过神来",
	"[逃避行为]\n{0}小时后，调查员恢复清醒时发现自己在很远的地方",
	"[恐惧]\n调查员患上一个新的恐惧症。调查员会在{0}小时后恢复正常，并开始为避开恐惧源而采取任何措施。\n{1}\n具体恐惧症: {2}(守秘人也可以自行从恐惧症状表中选择其他症状)",
	"[躁狂]\n调查员患上一个新的躁狂症，在{0}小时后恢复理智。在这次疯狂发作中，调查员将完全沉浸于其新的躁狂症状。这是否会被其他人理解（apparent to other people）则取决于守秘人和此调查员。\n{1}\n具体躁狂症: {2}(守秘人也可以自行从躁狂症状表中选择其他症状)"
};

//肉体天赋
static std::string PhysicalTalent[11]{
"",
"[明察秋毫]\n进行侦查检定时获得一个奖励骰。",
"[快速愈合]\n自然回复增加至每日3HP。",
"[昏暗视觉]\n降低夜间侦查检定的难度等级，忽略在夜间射击时的惩罚骰。",
"[耐力卓绝]\n进行体质检定时获得一个奖励骰，包括建立追逐时。",
"[天生神力]\n进行力量检定时获得一个奖励骰，比如用来举起某人某物。",
"[千杯不醉]\n可以花费5点幸运来避免过度饮酒带来的效果（无视技能惩罚）。",
"[强健体魄]\n可以花费10点幸运使疾病和毒药的伤害和效果减半。",
"[铁骨铮铮]\n可以花费10点幸运来吸收在一轮中收到的5点伤害。",
"[耳听八方]\n进行聆听检定时获得一个奖励骰。",
"[魅力四射]\n进行魅惑检定时获得一个奖励骰。",
};

//精神天赋
static std::string MentalTalent[11]{
"",
"[坚定不移]\n无视攻击人类、目睹惨烈创伤或尸体的理智损失。",
"[百折不挠]\n可以花费幸运来避免等量的理智损失。",
"[钢铁意志]\n进行意志检定时获得一个奖励骰。",
"[一目十行]\n阅读书籍和神话典籍时，泛读和精读花费的时间减半。",
"[语言学家]\n可以了解遇到的是哪种语言或文字；进行语言检定时获得一个奖励骰。",
"[魔法亲和]\n学习法术花费的时间减半；进行施法检定时获得一个奖励骰。",
"[过目不忘]\n能够记住事件的诸多细节；进行知识(教育)检定时获得一个奖励骰。",
"[博学多才]\n获得学问技能的一个专攻项，如梦境学问、吸血鬼学问、狼人学问；需要向该技能分配职业或兴趣点数。",
"[灵能觉醒]\n获得一项灵能，遥视、占卜、灵媒、心灵感应、念动其中之一，见第六章；需要向该技能分配职业或兴趣点数。",
"[足智多谋]\n能够迅速整理线索；进行智力（不是灵感）检定时获得一个奖励骰。",
};

//战斗天赋
static std::string CombatTalent[11]{
"",
"[处变不惊]\n不会被突袭。",
"[专注打击]\n在格斗中，可以花费10点幸运来获得额外伤害骰，数量取决于所用武器。如徒手攻击+1D3，剑+1D6。",
"[快速装填]\n选择一种武器，忽略使用该武器在同一回合装填并击发产生的惩罚骰。",
"[身手敏捷]\n应对枪械而寻找掩体时，不会失去攻击机会。",
"[目光如炬]\n忽略瞄准体型较小目标(体格-2)时产生的惩罚骰；忽略瞄准近战中的目标时产生的惩罚骰。",
"[技巧卓绝]\n使用战技时，角色的体格视为+1。",
"[疾风连击]\n在格斗中，可以花费10点幸运再进行一次攻击。",
"[动如脱兔]\n在一整场战斗中，可以花费10点幸运来避免寡不敌众。",
"[快速拔枪]\n决定回合轮次时，即使未准备好进行射击，亦视为获得+50DEX。",
"[手枪专精]\n忽略手枪连射带来的惩罚骰。",
};

//其他天赋
static std::string MiscellaneousTalent[11]{
"",
"[凶神恶煞]\n进行恐吓检定时降低一级难度等级，或者获得一个奖励骰，由守秘人判断。",
"[奇妙道具]\n游戏开始时获得一个奇妙道具，见怪奇技术，86页。",
"[吉人天相]\n回复幸运时，额外投一个1D10。",
"[神话知识]\n游戏开始时获得10点克苏鲁神话技能。",
"[怪奇技术]\n可以制造和修理怪奇技术制品，见怪奇技术，86页。",
"[遁入暗影]\n进行潜行检定时降低一级难度等级，或者获得一个奖励骰，由守秘人判断。如果目标未能察觉，在暴露之前可以进行两次突袭。",
"[能工巧匠]\n进行操作重型机械、机械维修和电气维修检定时降低一级难度等级，或者获得一个奖励骰，由守秘人判断。",
"[动物朋友]\n游戏开始时获得一只可靠的动物伙伴，比如猫、狗、鹦鹉；进行驯兽检定时获得一个奖励骰。",
"[伪装大师]\n进行乔装或技艺(表演)检定时，可以花费10点幸运来获得一个奖励骰；可以使用腹语，让声音听起来是从别处发出的；如果有人试图看穿伪装，其侦查或心理学检定提升为困难难度。",
"[早有准备]\n需要的东西似乎总在手边；可以花费10点幸运（而非幸运检定）在附近找到有用的道具，如手电筒、够长的绳索、武器等。",
};

//疯狂天赋
static std::string InsaneTalent[21]{
"",
"[疯狂力量]\n“我激发了自己所有的潜力！”在力量检定上获得一个奖励骰。如果检定失败，有些事就会变得糟糕。/n由守秘人来选择：英雄受伤（受到由于肌肉拉伤之类造成的1D3+英雄的伤害加值的伤害），或者者英雄正在处理的事物被打破了。",
"[疯狂敏捷]\n“我的手快到你看不见！”在敏捷检定上获得一个奖励骰。如果检定失败，有些事就会变得糟糕。/n由守秘人来选择：英雄受伤（受到1D4点伤害），或者他们把自己正在处理的东西给弄坏了。",
"[疯狂意志]\n“我感到强大的意志力涌入我的身体！”在意志检定上获得一个奖励骰。如果检定失败，有些事就会变得糟糕。/n由守秘人来选择：英雄昏迷，或者他们正在尝试达成的效果比预期更加的危险。",
"[疯狂体质]\n“我咬紧牙关，感觉不到疼痛！”当受到大量伤害时。英雄可以选择进行一次体质检定。如果成功，他们忍受住了痛苦并且将伤害减半。如果检定失败，他们受到原本应当骰出的伤害并摔倒在地，1D3回合内无法行动。",
"[疯狂外貌]\n“天哪，我看起来酷毙了宝贝儿！”英雄不知为何变得非常不一样了：这可能纯粹是面部表情和姿态上的变化，也可以是他们花时间把自己的服装发型改成了完全不一样的类型（衣服不会神奇地自动发生变化——英雄们必须自己手动更换）。在外貌检定或可以受外貌影响的检定上获得一个奖励骰，例如魅惑或话术。这个效果持续时间短暂但是可以作用于一段时间内的所有交流，例如一个场景或一次会议。如果英雄带有奖励骰的“改良”外貌检定失败，他们要遭受某种形式的社交耻辱或其他后果。",
"[疯狂记忆]\n“我完美地记住了所有事情！”英雄所有的记忆和曾经听说的事实都信手拈来。长相，人物，和种种细节在一阵庞大的信息中蜂拥而入他们的脑海之中。在教育、知识或当英雄们尝试回忆起自己可能听过见过的信息时进行的技能检定上获得一个奖励骰。如果检定失败，这些信息会变得过于庞大！丧失1点理智值并陷入疯狂发作。如果此时英雄不处于疯狂之中，他们会陷入临时性疯狂。",
"[疯狂速度]\n“快看我——我比子弹还要快！”当进入追逐轮时，英雄在决定他们移动力的体质检定上获得一个奖励骰。如果检定成功，他们的速度提高1点。如果检定结果是一次极难成功，他们的速度将提高2点。如果检定失败，他们会不知因何犯下错误并失去至少1D3回合的行动能力。",
"[疯狂驾驶]\n“现在没有什么能阻挡我了！”在一次追逐中英雄所有的驾驶检定都会获得一个奖励骰。如果检定失败，他们不知为何失去了对其载具的控制（由守秘人决定）。",
"[疯狂语言]\n“没，我从来没学过斯瓦希里语，但那能有多难？”英雄在短时间内暂时掌握了所有现代语言——或者一种古代语言或神话语言。这一效果持续的时间足够对一部巨著进行一次泛读、进行一次对话交流或者聆听一整段演讲。这一效果会将相应的语言技能变为75%。使用这种新语言需要进行一次技能检定，检定失败意味着英雄在1D6天内忘记了他们的母语，将其替换为当时他们正在使用的语言。",
"[疯狂精准]\n“看起来我就是打不偏啊！”英雄在所有射击技能检定上获得一个奖励骰直到他们的枪打空弹药。他们可以持续使用奖励骰直到其中一发射击没有击中目标或他们用光了弹药为止。打偏的一发射击会打到他们不想击中的东西——一名同伴或是某件具有重要价值的物品，并造成相当于极难成功的伤害（贯穿）。",
"[疯狂恐吓]\n“你小子觉得我很好笑吗？给我说说看哪里好笑？”英雄在恐吓检定都获得一个奖励骰。如果检定失败，他们将在短时间内失去对自己行为的控制。守秘人来决定会发生什么：要么其突然陷入暴怒（很可能对其正在交谈的对象造成伤害），要么他们被对方小瞧并羞辱了一番。",
"[疯狂闪避]\n“像蝴蝶一样飞舞！”英雄在目前的战斗场景中所有的闪避检定都获得一个奖励骰，直到其在一次闪避检定中失败。失败的检定意味着他们正好迎向对手的攻击，这种情况下其受到相当于敌人造成了一次极难成功时的伤害。",
"[疯狂指引]\n“跟着我，它就在那儿！”英雄的玩家告诉守秘人他们想去哪里或者朝着什么前进。守秘人给出前往那里的方向来实现这一点。英雄进行一次幸运检定：如果检定失败，他们会直接走进一个陷阱或者遇到某种危险的遭遇。",
"[疯狂通晓]\n“哦，现在我完全懂了！”英雄的玩家可以向守秘人提出一个有关黑幕的问题：“为什么敌人们在做……？”“敌人们做某事是为了……？”“要阻止敌人的计划我们能做的最佳方案是什么？”“敌人最大的弱点是什么？”问题应当十分具体，并且守秘人应当诚实地回答这一问题。这一天赋只能被使用一次——之后英雄们就会失去它。",
"[疯狂视野]\n“照明？谁需要那玩意？”英雄在侦查检定上获得一个奖励骰。即使是在完全的黑暗之中，英雄也可以像在微光下一样进行检定。/n如果检定失败，由守秘人来决定后果：双眼变得十分疼痛和过于敏感（其实是使其失明1D10回合），或者英雄将在接下来的一小时内持续看到幻觉。",
"[疯狂听觉]\n“大家静一静，什么东西在响？”英雄在聆听检定上获得一个奖励骰。无视环境中的噪音和其他声音，他们可以听到哪怕是最安静的声音。/n如果检定失败，由守秘人决定会发生什么：某种突然出现的噪音让他们失聪1D10分钟，或者英雄将在接下来一小时内持续的幻听。",
"[疯狂潜行]\n“你没看见我，对吧？”英雄在潜行检定上获得一个奖励骰。使其像猫一样优雅地移动，甚至能在毫无遮蔽的地方藏匿起来。/n如果检定失败，他们意外地打碎了某样东西或引起了巨大的骚动。",
"[疯狂残暴]\n“我要把你撕成碎片！”英雄所有的近战伤害检定进行两次并取最高的结果。代价是一旦他们开始攻击，他们就停不下来！他们在做出致命一击之前必须继续攻击。/n只有两种方法能够阻止这种情况，让他们失去意识或某人成功进行一次困难难度的话术、魅惑或恐吓检定（在一个战斗轮中只有一个人能尝试其中的一项检定）。",
"[疯狂技能强化]\n“你觉得我疯了是吧？但我告诉你这就是可能的！”作为疯狂的额外效果，英雄可以让他们的一项技能在克苏鲁神话的某方面获得强化（由玩家选择，经守秘人同意）。这会影响到技能能达成的效果范围。",
"[疯狂技能强化]\n“你觉得我疯了是吧？但我告诉你这就是可能的！”作为疯狂的额外效果，英雄可以让他们的一项技能在克苏鲁神话的某方面获得强化（由玩家选择，经守秘人同意）。这会影响到技能能达成的效果范围。",
};

//施法失败
static std::string FailedCasting[21]{
"",
"骇人的电闪雷鸣。"
"暂时失明1D6={0}轮（调整相应检定的难度等级/承受惩罚骰）。"
"自不知何处吹起一阵狂风（进行幸运检定来抓住纸张与书籍等易被吹走的物品）。"
"亮绿色的黏液自墙壁、地板或窗户等场所涌出（0/1D3理智损失）。"
"由守秘人决定的奇异幻觉（并附带适当的理智损失）。"
"附近的小动物爆炸（0/1D3理智损失）。"
"施法者的头发变成白色。"
"响亮却飘渺的尖叫（0/1理智损失）。"
"眼睛流血1D4={0}轮（调整相应检定的难度等级/承受惩罚骰）。"
"硫磺的恶臭。"
"大地震颤，墙壁崩塌。"
"施法者的手变得干枯而焦黑（进行幸运检定来决定是哪只手）并损失1D2生命值。/n根据守秘人的判断，这只手可能仅仅是暂时被烧焦（在所有DEX与需要用到这只手的 检定上承受惩罚骰）或永远地变成了这副干枯且漆黑的模样（扣除DEX与所有需要用到这只手的技能20点）。"
"自天空降下血雨，持续1D6={0}轮。"
"施法者不自然地衰老（+2D10={0}岁并调整相应的属性）。"
"施法者的皮肤永久性地半透明（见到施法者需损失1/1D4理智值）。"
"施法者获得1D10={0}点POW但同时失去1D10={0}点理智值。"
"一只神话生物被意外地召唤了出来。"
"守秘人随机挑选两项咒文并立即在此处发挥作用（以施法者为中心）。"
"施法者与附近的所有人被传送到了另一处地方（守秘人决定前往何处）。"
"一个神祇被意外地召唤了"
};

//恐惧
static std::string strFear[101] = {
	"",
	"[洗澡恐惧症（Ablutophobia）]\n对于洗涤或洗澡的恐惧。",
	"[恐高症（Acrophobia）]\n对于身处高处的恐惧。",
	"[飞行恐惧症（Aerophobia）]\n对飞行的恐惧。",
	"[广场恐惧症（Agoraphobia）]\n对于开放的（拥挤）公共场所的恐惧。",
	"[恐鸡症（Alektorophobia）]\n对鸡的恐惧。",
	"[大蒜恐惧症（Alliumphobia）]\n对大蒜的恐惧。",
	"[乘车恐惧症（Amaxophobia）]\n对于乘坐地面载具的恐惧。",
	"[恐风症（Ancraophobia）]\n对风的恐惧。",
	"[男性恐惧症（Androphobia）]\n对于成年男性的恐惧。",
	"[恐英症（Anglophobia）]\n对英格兰或英格兰文化的恐惧。",
	"[恐花症（Anthophobia）]\n对花的恐惧。",
	"[截肢者恐惧症（Apotemnophobia）]\n对截肢者的恐惧。",
	"[蜘蛛恐惧症（Arachnophobia）]\n对蜘蛛的恐惧。",
	"[闪电恐惧症（Astraphobia）]\n对闪电的恐惧。",
	"[废墟恐惧症（Atephobia）]\n对遗迹或残址的恐惧。",
	"[长笛恐惧症（Aulophobia）]\n对长笛的恐惧。",
	"[细菌恐惧症（Bacteriophobia）]\n对细菌的恐惧。",
	"[导弹/子弹恐惧症（Ballistophobia）]\n对导弹或子弹的恐惧。",
	"[跌落恐惧症（Basophobia）]\n对于跌倒或摔落的恐惧。",
	"[书籍恐惧症（Bibliophobia）]\n对书籍的恐惧。",
	"[植物恐惧症（Botanophobia）]\n对植物的恐惧。",
	"[美女恐惧症（Caligynephobia）]\n对美貌女性的恐惧。",
	"[寒冷恐惧症（Cheimaphobia）]\n对寒冷的恐惧。",
	"[恐钟表症（Chronomentrophobia）]\n对于钟表的恐惧。",
	"[幽闭恐惧症（Claustrophobia）]\n对于处在封闭的空间中的恐惧。",
	"[小丑恐惧症（Coulrophobia）]\n对小丑的恐惧。",
	"[恐犬症（Cynophobia）]\n对狗的恐惧。",
	"[恶魔恐惧症（Demonophobia）]\n对邪灵或恶魔的恐惧。",
	"[人群恐惧症（Demophobia）]\n对人群的恐惧。",
	"[牙科恐惧症（Dentophobia）]\n对牙医的恐惧。",
	"[丢弃恐惧症（Disposophobia）]\n对于丢弃物件的恐惧（贮藏癖）。",
	"[皮毛恐惧症（Doraphobia）]\n对动物皮毛的恐惧。",
	"[过马路恐惧症（Dromophobia）]\n对于过马路的恐惧。",
	"[教堂恐惧症（Ecclesiophobia）]\n对教堂的恐惧。",
	"[镜子恐惧症（Eisoptrophobia）]\n对镜子的恐惧。",
	"[针尖恐惧症（Enetophobia）]\n对针或大头针的恐惧。",
	"[昆虫恐惧症（Entomophobia）]\n对昆虫的恐惧。",
	"[恐猫症（Felinophobia）]\n对猫的恐惧。",
	"[过桥恐惧症（Gephyrophobia）]\n对于过桥的恐惧。",
	"[恐老症（Gerontophobia）]\n对于老年人或变老的恐惧。",
	"[恐女症（Gynophobia）]\n对女性的恐惧。",
	"[恐血症（Haemaphobia）]\n对血的恐惧。",
	"[宗教罪行恐惧症（Hamartophobia）]\n对宗教罪行的恐惧。",
	"[触摸恐惧症（Haphophobia）]\n对于被触摸的恐惧。",
	"[爬虫恐惧症（Herpetophobia）]\n对爬行动物的恐惧。",
	"[迷雾恐惧症（Homichlophobia）]\n对雾的恐惧。",
	"[火器恐惧症（Hoplophobia）]\n对火器的恐惧。",
	"[恐水症（Hydrophobia）]\n对水的恐惧。",
	"[催眠恐惧症（Hypnophobia）]\n对于睡眠或被催眠的恐惧。",
	"[白袍恐惧症（Iatrophobia）]\n对医生的恐惧。",
	"[鱼类恐惧症（Ichthyophobia）]\n对鱼的恐惧。",
	"[蟑螂恐惧症（Katsaridaphobia）]\n对蟑螂的恐惧。",
	"[雷鸣恐惧症（Keraunophobia）]\n对雷声的恐惧。",
	"[蔬菜恐惧症（Lachanophobia）]\n对蔬菜的恐惧。",
	"[噪音恐惧症（Ligyrophobia）]\n对刺耳噪音的恐惧。",
	"[恐湖症（Limnophobia）]\n对湖泊的恐惧。",
	"[机械恐惧症（Mechanophobia）]\n对机器或机械的恐惧。",
	"[巨物恐惧症（Megalophobia）]\n对于庞大物件的恐惧。",
	"[捆绑恐惧症（Merinthophobia）]\n对于被捆绑或紧缚的恐惧。",
	"[流星恐惧症（Meteorophobia）]\n对流星或陨石的恐惧。",
	"[孤独恐惧症（Monophobia）]\n对于一人独处的恐惧。",
	"[不洁恐惧症（Mysophobia）]\n对污垢或污染的恐惧。",
	"[黏液恐惧症（Myxophobia）]\n对黏液（史莱姆）的恐惧。",
	"[尸体恐惧症（Necrophobia）]\n对尸体的恐惧。",
	"[数字8恐惧症（Octophobia）]\n对数字8的恐惧。",
	"[恐牙症（Odontophobia）]\n对牙齿的恐惧。",
	"[恐梦症（Oneirophobia）]\n对梦境的恐惧。",
	"[称呼恐惧症（Onomatophobia）]\n对于特定词语的恐惧。",
	"[恐蛇症（Ophidiophobia）]\n对蛇的恐惧。",
	"[恐鸟症（Ornithophobia）]\n对鸟的恐惧。",
	"[寄生虫恐惧症（Parasitophobia）]\n对寄生虫的恐惧。",
	"[人偶恐惧症（Pediophobia）]\n对人偶的恐惧。",
	"[吞咽恐惧症（Phagophobia）]\n对于吞咽或被吞咽的恐惧。",
	"[药物恐惧症（Pharmacophobia）]\n对药物的恐惧。",
	"[幽灵恐惧症（Phasmophobia）]\n对鬼魂的恐惧。",
	"[日光恐惧症（Phenogophobia）]\n对日光的恐惧。",
	"[胡须恐惧症（Pogonophobia）]\n对胡须的恐惧。",
	"[河流恐惧症（Potamophobia）]\n对河流的恐惧。",
	"[酒精恐惧症（Potophobia）]\n对酒或酒精的恐惧。",
	"[恐火症（Pyrophobia）]\n对火的恐惧。",
	"[魔法恐惧症（Rhabdophobia）]\n对魔法的恐惧。",
	"[黑暗恐惧症（Scotophobia）]\n对黑暗或夜晚的恐惧。",
	"[恐月症（Selenophobia）]\n对月亮的恐惧。",
	"[火车恐惧症（Siderodromophobia）]\n对于乘坐火车出行的恐惧。",
	"[恐星症（Siderophobia）]\n对星星的恐惧。",
	"[狭室恐惧症（Stenophobia）]\n对狭小物件或地点的恐惧。",
	"[对称恐惧症（Symmetrophobia）]\n对对称的恐惧。",
	"[活埋恐惧症（Taphephobia）]\n对于被活埋或墓地的恐惧。",
	"[公牛恐惧症（Taurophobia）]\n对公牛的恐惧。",
	"[电话恐惧症（Telephonophobia）]\n对电话的恐惧。",
	"[怪物恐惧症（Teratophobia）]\n对怪物的恐惧。",
	"[深海恐惧症（Thalassophobia）]\n对海洋的恐惧。",
	"[手术恐惧症（Tomophobia）]\n对外科手术的恐惧。",
	"[十三恐惧症（Triskadekaphobia）]\n对数字13的恐惧症。",
	"[衣物恐惧症（Vestiphobia）]\n对衣物的恐惧。",
	"[女巫恐惧症（Wiccaphobia）]\n对女巫与巫术的恐惧。",
	"[黄色恐惧症（Xanthophobia）]\n对黄色或“黄”字的恐惧。",
	"[外语恐惧症（Xenoglossophobia）]\n对外语的恐惧。",
	"[异域恐惧症（Xenophobia）]\n对陌生人或外国人的恐惧。",
	"[动物恐惧症（Zoophobia）]\n对动物的恐惧。"
};
static std::string strPanic[101] = {
	"",
	"[沐浴癖（Ablutomania）]\n执着于清洗自己。",
	"[犹豫癖（Aboulomania）]\n病态地犹豫不定。",
	"[喜暗狂（Achluomania）]\n对黑暗的过度热爱。",
	"[喜高狂（Acromaniaheights）]\n狂热迷恋高处。",
	"[亲切癖（Agathomania）]\n病态地对他人友好。",
	"[喜旷症（Agromania）]\n强烈地倾向于待在开阔空间中。",
	"[喜尖狂（Aichmomania）]\n痴迷于尖锐或锋利的物体。",
	"[恋猫狂（Ailuromania）]\n近乎病态地对猫友善。",
	"[疼痛癖（Algomania）]\n痴迷于疼痛。",
	"[喜蒜狂（Alliomania）]\n痴迷于大蒜。",
	"[乘车癖（Amaxomania）]\n痴迷于乘坐车辆。",
	"[欣快癖（Amenomania）]\n不正常地感到喜悦。",
	"[喜花狂（Anthomania）]\n痴迷于花朵。",
	"[计算癖（Arithmomania）]\n狂热地痴迷于数字。",
	"[消费癖（Asoticamania）]\n鲁莽冲动地消费。",
	"[隐居癖（Automania）]\n过度地热爱独自隐居。",
	"[芭蕾癖（Balletmania）]\n痴迷于芭蕾舞。",
	"[窃书癖（Biliokleptomania）]\n无法克制偷窃书籍的冲动。",
	"[恋书狂（Bibliomania）]\n痴迷于书籍和/或阅读",
	"[磨牙癖（Bruxomania）]\n无法克制磨牙的冲动。",
	"[灵臆症（Cacodemomania）]\n病态地坚信自己已被一个邪恶的灵体占据。",
	"[美貌狂（Callomania）]\n痴迷于自身的美貌。",
	"[地图狂（Cartacoethes）]\n在何时何处都无法控制查阅地图的冲动。",
	"[跳跃狂（Catapedamania）]\n痴迷于从高处跳下。",
	"[喜冷症（Cheimatomania）]\n对寒冷或寒冷的物体的反常喜爱。",
	"[舞蹈狂（Choreomania）]\n无法控制地起舞或发颤。",
	"[恋床癖（Clinomania）]\n过度地热爱待在床上。",
	"[恋墓狂（Coimetormania）]\n痴迷于墓地。",
	"[色彩狂（Coloromania）]\n痴迷于某种颜色。",
	"[小丑狂（Coulromania）]\n痴迷于小丑。",
	"[恐惧狂（Countermania）]\n执着于经历恐怖的场面。",
	"[杀戮癖（Dacnomania）]\n痴迷于杀戮。",
	"[魔臆症（Demonomania）]\n病态地坚信自己已被恶魔附身。",
	"[抓挠癖（Dermatillomania）]\n执着于抓挠自己的皮肤。",
	"[正义狂（Dikemania）]\n痴迷于目睹正义被伸张。",
	"[嗜酒狂（Dipsomania）]\n反常地渴求酒精。",
	"[毛皮狂（Doramania）]\n痴迷于拥有毛皮。",
	"[赠物癖（Doromania）]\n痴迷于赠送礼物。",
	"[漂泊症（Drapetomania）]\n执着于逃离。",
	"[漫游癖（Ecdemiomania）]\n执着于四处漫游。",
	"[自恋狂（Egomania）]\n近乎病态地以自我为中心或自我崇拜。",
	"[职业狂（Empleomania）]\n对于工作的无尽病态渴求。",
	"[臆罪症（Enosimania）]\n病态地坚信自己带有罪孽。",
	"[学识狂（Epistemomania）]\n痴迷于获取学识。",
	"[静止癖（Eremiomania）]\n执着于保持安静。",
	"[乙醚上瘾（Etheromania）]\n渴求乙醚。",
	"[求婚狂（Gamomania）]\n痴迷于进行奇特的求婚。",
	"[狂笑癖（Geliomania）]\n无法自制地，强迫性的大笑。",
	"[巫术狂（Goetomania）]\n痴迷于女巫与巫术。",
	"[写作癖（Graphomania）]\n痴迷于将每一件事写下来。",
	"[裸体狂（Gymnomania）]\n执着于裸露身体。",
	"[妄想狂（Habromania）]\n近乎病态地充满愉快的妄想（而不顾现实状况如何）。",
	"[蠕虫狂（Helminthomania）]\n过度地喜爱蠕虫。",
	"[枪械狂（Hoplomania）]\n痴迷于火器。",
	"[饮水狂（Hydromania）]\n反常地渴求水分。",
	"[喜鱼癖（Ichthyomania）]\n痴迷于鱼类。",
	"[图标狂（Iconomania）]\n痴迷于图标与肖像",
	"[偶像狂（Idolomania）]\n痴迷于甚至愿献身于某个偶像。",
	"[信息狂（Infomania）]\n痴迷于积累各种信息与资讯。",
	"[射击狂（Klazomania）]\n反常地执着于射击。",
	"偷窃癖（Kleptomania）]\n反常地执着于偷窃。",
	"[噪音癖（Ligyromania）]\n无法自制地执着于制造响亮或刺耳的噪音。",
	"[喜线癖（Linonomania）]\n痴迷于线绳。",
	"[彩票狂（Lotterymania）]\n极端地执着于购买彩票。",
	"[抑郁症（Lypemania）]\n近乎病态的重度抑郁倾向。",
	"[巨石狂（Megalithomania）]\n当站在石环中或立起的巨石旁时，就会近乎病态地写出各种奇怪的创意。",
	"[旋律狂（Melomania）]\n痴迷于音乐或一段特定的旋律。",
	"[作诗癖（Metromania）]\n无法抑制地想要不停作诗。",
	"[憎恨癖（Misomania）]\n憎恨一切事物，痴迷于憎恨某个事物或团体。",
	"[偏执狂（Monomania）]\n近乎病态地痴迷与专注某个特定的想法或创意。",
	"[夸大癖（Mythomania）]\n以一种近乎病态的程度说谎或夸大事物。",
	"[臆想症（Nosomania）]\n妄想自己正在被某种臆想出的疾病折磨。",
	"[记录癖（Notomania）]\n执着于记录一切事物（例如摄影）",
	"[恋名狂（Onomamania）]\n痴迷于名字（人物的、地点的、事物的）",
	"[称名癖（Onomatomania）]\n无法抑制地不断重复某个词语的冲动。",
	"[剔指癖（Onychotillomania）]\n执着于剔指甲。",
	"[恋食癖（Opsomania）]\n对某种食物的病态热爱。",
	"[抱怨癖（Paramania）]\n一种在抱怨时产生的近乎病态的愉悦感。",
	"[面具狂（Personamania）]\n执着于佩戴面具。",
	"[幽灵狂（Phasmomania）]\n痴迷于幽灵。",
	"[谋杀癖（Phonomania）]\n病态的谋杀倾向。",
	"[渴光癖（Photomania）]\n对光的病态渴求。",
	"[背德癖（Planomania）]\n病态地渴求违背社会道德",
	"[求财癖（Plutomania）]\n对财富的强迫性的渴望。",
	"[欺骗狂（Pseudomania）]\n无法抑制的执着于撒谎。",
	"[纵火狂（Pyromania）]\n执着于纵火。",
	"[提问狂（Question-asking Mania）]\n执着于提问。",
	"[挖鼻癖（Rhinotillexomania）]\n执着于挖鼻子。",
	"[涂鸦癖（Scribbleomania）]\n沉迷于涂鸦。",
	"[列车狂（Siderodromomania）]\n认为火车或类似的依靠轨道交通的旅行方式充满魅力。",
	"[臆智症（Sophomania）]\n臆想自己拥有难以置信的智慧。",
	"[科技狂（Technomania）]\n痴迷于新的科技。",
	"[臆咒狂（Thanatomania）]\n坚信自己已被某种死亡魔法所诅咒。",
	"[臆神狂（Theomania）]\n坚信自己是一位神灵。",
	"[抓挠癖（Titillomaniac）]\n抓挠自己的强迫倾向。",
	"[手术狂（Tomomania）]\n对进行手术的不正常爱好。",
	"[拔毛癖（Trichotillomania）]\n执着于拔下自己的头发。",
	"[臆盲症（Typhlomania）]\n病理性的失明。",
	"[嗜外狂（Xenomania）]\n痴迷于异国的事物。",
	"[喜兽癖（Zoomania）]\n对待动物的态度近乎疯狂地友好。"
};

#endif /*JRS_RD_CONSTANT*/
