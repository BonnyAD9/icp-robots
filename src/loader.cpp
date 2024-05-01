#include "loader.hpp"

namespace icp {

using namespace std;

Loader::Loader(string filename)
    : cur('\0'),
    filename(filename)
{}

Room *Loader::load(QWidget *window) {
    auto room = new Room();

    file = ifstream(filename);
    if (!file.is_open())
        throw runtime_error("File cannot be accessed");

    auto ident = read_ident();
    while (ident != "") {
        if (ident == "room") {
            auto size = read_size();
            window->resize(size.width(), size.height() + 80);
        } else if (ident == "obstacle") {
            room->add_obstacle(unique_ptr<Obstacle>(load_obstacle()));
        } else if (ident == "robot") {
            room->add_robot(unique_ptr<Robot>(load_robot()));
        } else if (ident == "auto_robot") {
            room->add_robot(unique_ptr<Robot>(load_auto_robot()));
        } else if (ident == "control_robot") {
            room->add_robot(unique_ptr<Robot>(load_control_robot()));
        } else {
            throw runtime_error("Unexpected identifier: '" + ident + "'");
        }
        ident = read_ident();
    }
    file.close();
    return room;
}

Obstacle *Loader::load_obstacle() {
    QPointF pos;
    QSizeF size;
    bool spos = false, ssize = false;

    while (!spos || !ssize) {
        if (!(file >> cur))
            throw runtime_error("Obstacle requires size and position");

        if (isdigit(cur) && !ssize) {
            file.seekg((int)file.tellg() - 1);
            size = read_size();
            ssize = true;
        } else if (cur == '[' && !spos) {
            pos = read_pos();
            spos = true;
        } else {
            throw runtime_error("Unexpected obstacle attribute");
        }
    }

    auto rect = QRectF(pos, size);
    auto obst = new Obstacle(rect);
    obst->set_hitbox(rect);
    return obst;
}

Robot *Loader::load_robot() {
    qreal speed = 0, angle = -90;
    QPointF pos;
    bool spos = false, sopt = false;

    while (!spos || !sopt) {
        if (!(file >> cur)) {
            if (spos)
                break;
            throw runtime_error("Robot requires position");
        }

        if (cur == '[') {
            pos = read_pos();
            spos = true;
        } else if (cur == '{') {
            while (true) {
                auto ident = read_ident();
                if (ident == "speed") {
                    file >> speed;
                } else if (ident == "angle") {
                    file >> angle;
                } else {
                    throw runtime_error(
                        "Unexpected robot attribute: '" + ident + "'"
                    );
                }

                file >> cur;
                if (cur == '}')
                    break;
                if (cur == ',')
                    continue;

                throw runtime_error("Unexpected character");
            }
        } else {
            file.seekg((int)file.tellg() - 1);
            sopt = true;
        }
    }

    angle = -angle * M_PI / 180.0;
    auto rob = new Robot(pos, angle, speed);
    rob->set_hitbox(QRectF(pos, QSizeF(0, 0)));
    return rob;
}

AutoRobot *Loader::load_auto_robot() {
    qreal speed = 0, angle = -90, el = 20, el_r = M_PI / M_E, r = M_PI / 4;
    QPointF pos;
    bool spos = false, sopt = false;

    while (!spos || !sopt) {
        if (!(file >> cur)) {
            if (spos)
                break;
            throw runtime_error("Robot requires position");
        }

        if (cur == '[') {
            pos = read_pos();
            spos = true;
        } else if (cur == '{') {
            while (true) {
                auto ident = read_ident();
                if (ident == "speed") {
                    file >> speed;
                } else if (ident == "angle") {
                    file >> angle;
                } else if (ident == "elide_distance") {
                    file >> el;
                } else if (ident == "elide_rotation") {
                    file >> el_r;
                } else if (ident == "rotation_speed") {
                    file >> r;
                } else {
                    throw runtime_error(
                        "Unexpected robot attribute: '" + ident + "'"
                    );
                }

                file >> cur;
                if (cur == '}')
                    break;
                if (cur == ',')
                    continue;

                throw runtime_error("Unexpected character");
            }
        } else {
            file.seekg((int)file.tellg() - 1);
            sopt = true;
        }
    }

    angle = -angle * M_PI / 180.0;
    auto rob = new AutoRobot(pos, angle, speed, el, el_r, r);
    rob->set_hitbox(QRectF(pos, QSizeF(0, 0)));
    return rob;
}

ControlRobot *Loader::load_control_robot() {
    qreal speed = 0, angle = -90, r = M_PI / 4;
    QPointF pos;
    bool spos = false, sopt = false;

    while (!spos || !sopt) {
        if (!(file >> cur)) {
            if (spos)
                break;
            throw runtime_error("Robot requires position");
        }

        if (cur == '[') {
            pos = read_pos();
            spos = true;
        } else if (cur == '{') {
            while (true) {
                auto ident = read_ident();
                if (ident == "speed") {
                    file >> speed;
                } else if (ident == "angle") {
                    file >> angle;
                } else if (ident == "rotation_speed") {
                    file >> r;
                } else {
                    throw runtime_error(
                        "Unexpected robot attribute: '" + ident + "'"
                    );
                }

                file >> cur;
                if (cur == '}')
                    break;
                if (cur == ',')
                    continue;

                throw runtime_error("Unexpected character");
            }
        } else {
            file.seekg((int)file.tellg() - 1);
            sopt = true;
        }
    }

    angle = -angle * M_PI / 180.0;
    auto rob = new ControlRobot(pos, angle, speed, r);
    rob->set_hitbox(QRectF(pos, QSizeF(0, 0)));
    return rob;
}

string Loader::read_ident() {
    string res = "";
    file >> ws;
    while (file.get(cur)) {
        if (!isalpha(cur) && cur != '_')
            break;
        res += cur;
    }

    if (cur == ':' || (cur == ' ' && file >> ws >> cur && cur == ':') || res == "")
        return res;
    throw runtime_error("Identifier must be followed by ':'");
}

QSizeF Loader::read_size() {
    qreal w, h;
    file >> w;
    if (!(file >> cur && cur == 'x'))
        throw runtime_error("Invalid character in size");

    file >> h;
    return QSizeF(w, h);
}

QPointF Loader::read_pos() {
    qreal x, y;
    file >> x;
    if (!(file >> cur && cur == ','))
        throw runtime_error("Invalid character in position");

    file >> y;
    if (!(file >> cur && cur == ']'))
        throw runtime_error("Unclosed position");

    return QPointF(x, y);
}

} // namespace icp
