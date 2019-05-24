#ifndef SRC_LIB_POINTERVECTOR_H_
#define SRC_LIB_POINTERVECTOR_H_

#include <vector>

/**
 * This is the base class for any vector that holds pointers to data we keep.
 * On destruction or a call to erase, we destroy not only our contents, but the
 * pointed-to values. If you pull something from us, the person pulling it owns it.
 *
 * It is possible to abuse this object by doing things like calling clear() directly.
 * Please don't.
 */
template <class ObjectType>
class PointerVector : public std::vector<ObjectType *> {
private:
	bool iOwnTheData;

public:
	PointerVector(): iOwnTheData(true) {}

	/**
	 * THe default behavior is the vector owns the data, but you can override that
	 * using this constructor. Note that this ONLY effects the clear() and eraseAll()
	 * methods plus the destructor.
	 */
	PointerVector(bool owns): iOwnTheData(owns) {}

	/**
	 * Destructor does an eraseAll().
	 */
	virtual ~PointerVector() { eraseAll(); }

	bool ownsTheData() const { return iOwnTheData; }

	/**
	 * Do we have this object?
	 */
	bool contains(ObjectType *ptr) {
		for (auto iter = this->begin(); iter != this->end(); ++iter) {
			if (*iter == ptr) {
				return true;
			}
		}
		return false;
	}

	int indexOf(ObjectType *ptr) {
		int retVal = 0;
		for (auto iter = this->begin(); iter != this->end(); ++iter) {
			if (*iter == ptr) {
				return retVal;
			}
			++retVal;
		}
		return -1;
	}

	/**
	 * This is an attempt to catch people who call clear() instead of eraseAll(),
	 * but if they're passing us around as a superclass, they can cheat, as the STL
	 * people, in their wisdom, don't make their methods virtual.
	 */
	void clear() {
		eraseAll();
	}

	/**
	 * Erase our contents, freeing any memory we're pointed to.
	 */
	void eraseAll() {
		if (iOwnTheData) {
			for (ObjectType *value: *this) {
				delete value;
			}
		}

		std::vector<ObjectType *>::clear();
	}

	/**
	 * This removes this object without deleting the object. The caller now owns the data.
	 */
	void remove(ObjectType *ptr) {
		for (auto iter = this->begin(); iter != this->end(); ++iter) {
			if (*iter == ptr) {
				this->erase(iter);
				break;
			}
		}
	}
};

#endif /* SRC_LIB_POINTERVECTOR_H_ */
