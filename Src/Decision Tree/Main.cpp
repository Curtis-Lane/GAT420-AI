#include "Node.h"

// Traverse root using recursion
void traverse(node_t* node);

int main() {
	bool isOnBreak = false;
	bool isSchoolDay = false;
	bool haveHomework = true;
	int daysUntilHomeworkDue = 3;

	node_t* throwOutSleepScheduleAction = new action_t("Throw out sleep schedule");
	node_t* getUpAtSevenAction = new action_t("Get up at 7");
	node_t* getUpAtNoonAction = new action_t("Try to get up at noon");
	node_t* sleepInAction = new action_t("Sleep in");

	node_t* root = new decision_t<bool>("Is On Break", isOnBreak, Predicate::EQUAL, true);
	node_t* schoolDayDecision = new decision_t<bool>("Is School Day", isSchoolDay, Predicate::EQUAL, true);
	node_t* homeworkDecision = new decision_t<bool>("Have homework", haveHomework, Predicate::EQUAL, true);
	node_t* homeworkDueSoonDecision = new decision_t<int>("Is homework due soon", daysUntilHomeworkDue, Predicate::LESS, 4);

	root->trueNode = throwOutSleepScheduleAction;
	root->falseNode = schoolDayDecision;

	schoolDayDecision->trueNode = getUpAtSevenAction;
	schoolDayDecision->falseNode = homeworkDecision;

	homeworkDecision->trueNode = homeworkDueSoonDecision;
	homeworkDecision->falseNode = sleepInAction;

	homeworkDueSoonDecision->trueNode = getUpAtNoonAction;
	homeworkDueSoonDecision->falseNode = sleepInAction;

	traverse(root);

	delete throwOutSleepScheduleAction;
	delete getUpAtSevenAction;
	delete getUpAtNoonAction;
	delete sleepInAction;

	delete root;
	delete schoolDayDecision;
	delete homeworkDecision;
	delete homeworkDueSoonDecision;

	return 0;
}

void traverse(node_t* node) {
	node_t* nextNode = node->execute();
	if(nextNode != nullptr) {
		traverse(nextNode);
	}
}