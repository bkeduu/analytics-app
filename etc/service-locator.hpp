#pragma once

#include <QObject>

template<typename T>
class ServiceLocator {

public:
	ServiceLocator() {

	}

	bool insert(const QString& key, const T& value) {
		if (!valueMap.contains(key)) {
			valueMap.insert(key, value);
			return true;
		}
		return false;
	}

	T value(const QString& key) {
		if (valueMap.contains(key)) {
			return valueMap[key];
		}
		return T();
	}

	QString key(const T& value) {
		return valueMap.key(value);
	}

	bool pop(const QString& key) {
		if (valueMap.contains(key)) {
			valueMap.remove(key);
			return true;
		}
		return false;
	}

	bool pop(const T& value) {
		QStringList keys = valueMap.keys(value);
		valueMap.remove(keys[0]);
		return !keys.empty();
	}

	bool contains(const QString& key) {
		return valueMap.contains(key);
	}

	bool contains(const T& value) {
		return !valueMap.keys(value).empty();
	}

private:
	QMap<QString, T> valueMap;

};
