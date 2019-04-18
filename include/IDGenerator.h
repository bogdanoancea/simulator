/*
 * Data simulator for mobile phone network events
 *
 * IDGenerator.h
 *
 *  Created on: Apr 11, 2019
 *      Author: Bogdan Oancea
 */

#ifndef IDGENERATOR_H_
#define IDGENERATOR_H_

#include <cstdint>

class IDGenerator {
	public:
		static IDGenerator* instance() {
			if (!m_instance) {
				m_instance = new IDGenerator;
			}
			return m_instance;
		}

		uint32_t next() {
			return m_id++;
		}

	private:
		IDGenerator() :
				m_id(0) {
		}
		static IDGenerator* m_instance;
		uint32_t m_id;

};

#endif /* IDGENERATOR_H_ */
