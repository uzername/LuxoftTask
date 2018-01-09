#pragma once
#include <memory>
#include <QAbstractListModel>

//that is not a logic, it is a middleware between qml and actual code. Constructed by Ancients
class Logic: public QAbstractListModel
{
    Q_OBJECT
public:

    enum GlobalConstants {
        BOARD_SIZE = 8
    };
    enum Roles {
        Type = Qt::UserRole,
        PositionX,
        PositionY,
    };
    
public:
    explicit Logic(QObject *parent = 0);
    ~Logic();

    Q_PROPERTY(int boardSize READ boardSize CONSTANT);
    int boardSize() const;

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool move(int fromX, int fromY, int toX, int toY);

protected:
    int rowCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
