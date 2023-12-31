﻿#include "TravelService.h"
#include <iostream>

long TravelService::profit = 0;

TravelService::TravelService(
	const std::string& name,
	const std::string& email,
	const std::string& telephone,
	int age,
	int balance)
{
	account = new Account(name, email, telephone, age, balance);
}

TravelService::TravelService() {
	account = new Account("", "", "", 0, 0);
}

TravelService::TravelService(const Account& account) {
	this->account = new Account();
	*this->account = account;
}

TravelService::~TravelService() {
	delete account;
}

// Статический метод, необходимый для создания экземпляра класса FlightBookingService через консоль.
TravelService* TravelService::createFromConsole() {
	TravelService* travel_service = new TravelService(Account::createFromConsole());
	return travel_service;
}

// Метод, распечатывающий информации обо всех существующих маршрутах.
void TravelService::printAvailableRouts() const {
	if (!routes.empty()) {
		std::cout << "Доступные маршруты:\n\n";
		std::size_t index = 1;

		for (const auto& route : routes) {
			std::cout << index++ << ". ";
			std::cout << route << std::endl;
		}
	}
	else {
		std::cout << "В данный момент нет доступных маршрутов.\n";
	}
}

// Метод, распечатывающий информацию об аккаунте.
void TravelService::printAccountInfo() const {
	std::cout << "Данные аккаунта:\n";
	std::cout << "ФИО: " << account->name << '\n';
	std::cout << "Возраст: " << account->age << '\n';
	std::cout << "Почта: " << account->email << '\n';
	std::cout << "Телефон: " << account->telephone << "\n\n";
}

// Метод, предназначенный для изменения информации профиля.
void TravelService::changeAccountInfo(
	const std::string& name,
	const std::string& email,
	const std::string& telephone,
	int age)
{
	account->name = name;
	account->email = email;
	account->telephone = telephone;
	account->age = age;
	std::cout << "Учетные данные Вашего аккаунта успешно изменены.\n\n";
}

// Метод, возвращающий баланс пользователя.
int TravelService::getBalance() const {
	return account->balance;
}

// Метод, предназначенный для пополнения баланса пользователя.
void TravelService::topUpBalance(int amount) {
	account->balance += amount;
	std::cout << "Ваш баланс успешно пополнен на " << amount << " рублей.\n";
	std::cout << "Теперь он составляет " << account->balance << " рублей.\n\n";
}

// Метод, предназначенный для добавления маршрутов.
void TravelService::addRoute(const Route& route) {
	for (const auto& rt : routes) {
		if (rt == route) {
			std::cout << "Данный маршрут уже существует, добавление невозможно.\n";
			return;
		}
	}
	routes.push_back(route);
	std::cout << "Маршрут успешно добавлен.\n";
}

// Метод, предназначенный для удаления маршрута.
void TravelService::removeRoute(std::size_t desired_ind) {
	if (routes.empty()) {
		std::cout << "Сейчас нет доступных маршрутов";
		return;
	}

	if (desired_ind < 1 || desired_ind > routes.size()) {
		std::cout << "Введен некорректный номер билета." << std::endl;
		return;
	}
	
	std::size_t index = 1;
	for (auto it = routes.begin(); it != routes.end(); it++, index++) {
		if (index == desired_ind) {
			routes.erase(it);
			std::cout << "Маршрут успешно удален.\n";
			return;
		}
	}
}

// Метод, производящий поиск маршрутов по выбранному городу.
// Выводит список всех доступных городов, город посадки которых совпадает с выбранным.
void TravelService::searchTicketsByCity(const std::string& desired_city) const {
	bool is_found = false;
	std::size_t index = 1;
	for (const auto& rt : routes) {
		if (rt.arrival_city == desired_city) {
			if (!is_found) {
				is_found = true;
				std::cout << "Найдены следующие маршруты до города " << desired_city << ":\n";
			}
			std::cout << index++ << ". ";
			std::cout << rt << std::endl;
		}
	}

	if (!is_found) {
		std::cout << "Подходящих маршрутов не найдено.\n";
	}
}

// Метод, производящий поиск маршрутов по заданной цене.
// Выводит список всех доступных городов, цена билетов которых не превышает заданную.
void TravelService::searchTicketsByPrice(int available_money) const {
	bool is_found = false;
	std::size_t index = 1;
	for (const auto& rt : routes) {
		if (rt.ticket_price <= available_money) {
			if (!is_found) {
				is_found = true;
				std::cout << "Найдены следующие маршруты стоимостью до " << available_money << " рублей:\n";
			}
			std::cout << index++ << ". ";
			std::cout << rt << std::endl;
		}
	}

	if (!is_found) {
		std::cout << "Подходящих маршрутов не найдено.\n";
	}
}

// Метод, предназначенный для покупки билетов.
void TravelService::buyTicket(const Route& route) const {
	bool is_not_enough_money = false;
	for (const auto& rt : routes) {
		if (rt == route) {
			if (account->balance >= rt.ticket_price) {
				account->tickets.push_back(rt);
				account->balance -= route.ticket_price;
				profit += route.ticket_price;
				std::cout << "Билет успешно куплен, на вашем счету "
					"осталось " << account->balance << " рублей.\n\n";
				return;
			}
			else {
				is_not_enough_money = true;
			}
		}
	}
	if (is_not_enough_money) {
		std::cout << "На Вашем счету недостаточно средств для покупки билета.\n\n";
	}
	else {
		std::cout << "Данный маршрут пока недоступен.\n\n";
	}
}

// Метод, предназначенный для продажи билета.
void TravelService::sellTicket(std::size_t desired_ind) {
	if (account->tickets.empty()) {
		std::cout << "На Вашем аккаунте нет купленных билетов.\n\n";
		return;
	}

	if (desired_ind < 1 || desired_ind > account->tickets.size()) {
		std::cout << "Введен некорректный номер купленного билета." << std::endl;
		return;
	}

	std::size_t index = 1;

	for (auto it = account->tickets.begin(); it != account->tickets.end(); it++, index++) {
		if (index == desired_ind) {
			account->balance += it->ticket_price;
			profit -= it->ticket_price;
			account->tickets.erase(it);
			break;
		}
	}

	std::cout << "Билет успешно продан, на Вашем счету " << account->balance << " рублей.\n\n";
	
}

// Метод, распечатывающий информацию о купленном билете.
void TravelService::printTicketsInfo() const {
	if (account->tickets.empty()) {
		std::cout << "На Вашем аккаунте нет купленных билетов.\n";
		return;
	}

	std::size_t index = 1;
	for (const auto& route : account->tickets) {
		std::cout << index++ << ". " << route << std::endl;
	}
}

// Метод, распечатывающий информацию о прибыли компании.
void TravelService::printCompanyProfit() {
	std::cout << "На данный момент прибыль компании составляет " << profit << " рублей.\n\n";
}
