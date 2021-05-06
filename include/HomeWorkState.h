/*
 * WorkHomeState.h
 *
 *  Created on: Mar 25, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_HOMEWORKSTATE_H_
#define INCLUDE_HOMEWORKSTATE_H_

/**
 * This class is an enumeration of states of a person in a home-work simulation scenario:
 * STAY_HOME
 * GO_WORK
 * STAY_WORK
 * GO_HOME
  */
enum class HomeWorkState {
	STAY_HOME, GO_WORK, STAY_WORK, GO_ANCHOR, STAY_ANCHOR, GO_HOME
};

#endif /* INCLUDE_HOMEWORKSTATE_H_ */
