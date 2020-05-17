#include <vector>
#include <QApplication>
#include <QTimer>
#include <iostream>
#include <unistd.h>
#include "Controller.h"

/*
 * Из контроллера осуществляется управление игровым процессом.
 * Изначально запускается окно с выбором количества игроков (runGame). При выборе посылается сигнал StateMachine::set_num_of_players,
 * после чего запускается окно с выбором скинов игроков Controller::set_num_of_players_for_lvl.
 * Когда все игроки готовы, (PlayerSelection::start_level -> StateMachine::start_level ->
 * StateMachine::set_level -> Controller::run_level) начинается уровень, который пока длится 10 секунд.
 * По окончании level_durance выводится статистика по уровню : Controller::end_level.
 * В появившемся окне статистики можно выбрать следующий режим игры.
 */

ConnectionUpdater::ConnectionUpdater(Inet::InternetConnection *&ic) : inetConnection_(ic) {
    std::cout << ic << std::endl;;
}

void ConnectionUpdater::commit() {
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(advance()));
    std::cout << "cyka" << std::endl;
    timer->start(10);
    timer->setInterval(16);
    std::cout << "OAOAO" << std::endl;
}

void ConnectionUpdater::advance() {
    if (inetConnection_) {
        inetConnection_->receive();
        // std::cout << "YESS" << std::endl;
    } else {
        // std::cout << "NOOO" << std::endl;
    }
}

Controller::Controller(int argc, char *argv[])
    : app(argc, argv)
    , scene_(new Scene())
    , key_presser_(new KeyPresser(internetConnection))
    , state_machine_(new StateMachine())
    , model_(new Model(scene_, state_machine_))
    , menu_(new Menu(scene_, state_machine_))
    // , key_presser_helper_(new KeyPresserHelper(key_presser_))
    , player_selection(new PlayerSelection(scene_))
    , connUpd_(new ConnectionUpdater(internetConnection)) {
    key_presser_->setFixedSize(QSize(scene_->get_width(), scene_->get_height()));
    scene_->addWidget(key_presser_);
    connect(state_machine_, &StateMachine::set_num_of_players, this, &Controller::set_num_of_players_for_lvl);
    connect(state_machine_, &StateMachine::set_exit_game, this, &Controller::exit_game);
    connect(player_selection, &PlayerSelection::start_level, state_machine_, &StateMachine::start_level);
    connect(state_machine_, &StateMachine::set_level, this, &Controller::run_level);
    connect(state_machine_, &StateMachine::set_end_level, this, &Controller::end_level);
    connect(state_machine_, &StateMachine::set_inet_type, this, &Controller::set_inet_type);
    connect(state_machine_, &StateMachine::begin_connection_upd, connUpd_, &ConnectionUpdater::commit);
}

Controller::~Controller() {
    player_selection->~PlayerSelection();
    menu_->~Menu();
}

static int connect(Inet::Client *client) { // ждет ответного пакета
    int attempts = 100;
    while (!client->id()) {
        if (attempts-- == 0) {
            return 0;
        }
        client->receive();
        usleep(160);
    }
    return client->id();
}

void Controller::set_inet_type() {
    std::string inetType;
    std::cout << "Enter \'server\' or \'client\' or \'offline\'" << std::endl;
    std::cin >> inetType;
    if (inetType == "server") {
        internetConnection = new Inet::Server();
	remoteClicker_ = new PlayerSelectionRemoteClicker(*player_selection, internetConnection);
    } else if (inetType == "client") {
        internetConnection = new Inet::Client();
        while (!localId) {
            std::cout << "Enter server port: ";
            unsigned short serverPort;
            std::cin >> serverPort;
            internetConnection->connect({127, 0, 0, 1, serverPort});
            localId = ::connect(static_cast<Inet::Client *>(internetConnection));
            if (!localId) {
                std::cout << "Could not connect. ";
            }
	    remoteClicker_ = new PlayerSelectionRemoteClicker(*player_selection, internetConnection);
        }
    } else if (inetType == "offline") {
        // nu, bivaet, delat' niche ne nado
    } else {
        std::cout << "Are you stupid? it's not server or client or offline, asshole" << std::endl;
        assert(false);
    }
}

/*
 * Запускает стартововый экран с выбором количества персонажей; get_cur_state = MENU_NUM_OF_PLAYERS
 */
int Controller::run_game() {
    menu_->run_menu(state_machine_->get_cur_state());
    return app.exec();
}

/*
 * Добавляет нужное количество персонажей и запускает окно с выбором текстур для персонажей
 */
void Controller::set_num_of_players_for_lvl(Utilities::GameNumOfPlayers num) {
    switch (num) {
        case Utilities::GameNumOfPlayers::SINGLE_PLAYER:
            players_.push_back(new Player(Utilities::Color::GREEN));
            key_presser_->add_players(players_[0]);
            break;
        case Utilities::GameNumOfPlayers::TWO_PLAYERS:
            players_.push_back(new Player(Utilities::Color::GREEN));
            players_.push_back(new Player(Utilities::Color::YELLOW));
            key_presser_->add_players(players_[localId], players_[1 - localId]); // потом тупо случаи как-то разбирать
            break;
    }
    run_player_selection();
}

/*
 * Прячет предыдущее окно меню. Открывается новое окно с выбором текстур.
 */
void Controller::run_player_selection() {
    menu_->clear_menu();
    only for 2 players; DOES NOT WORK
    if (internetConnection && internetConnection->id() == 1) {
    	std::swap(players_[1], players_[0]);
    }
    player_selection->add_players(players_);
    if (internetConnection && internetConnection->id() == 1) {
    	std::swap(players_[1], players_[0]);
    }
    player_selection->run_player_selection();
}

/*
 * Когда все игроки готовы, начинает уровень.
 */
void Controller::run_level(Utilities::GameMode mode) {
    player_selection->clear_player_selection();
    menu_->clear_menu();
    model_->add_players(players_);
    model_->set_statistics();
    model_->make_new_level();
    level_durance = new QTimer(this);
    connect(level_durance, SIGNAL(timeout()), state_machine_, SLOT(end_level()));
    connect(level_durance, SIGNAL(timeout()), model_, SLOT(stop_advance_scene()));
    level_durance->start(70000);
}

/*
 * По истечении времени останавливает таймер, останавливает игроков, выводит статистику.
 */
void Controller::end_level() {
    level_durance->stop();
    model_->show_statistics();
}

/*
 * При выборе выхода из игры заканчивает процессы, выходит из приложения.
 */
void Controller::exit_game() {
    app.quit();
}
