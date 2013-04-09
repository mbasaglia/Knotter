#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <QObject>

/**
 * Manage resources and data
 */
class Resource_Manager : public QObject
{
    Q_OBJECT

    Resource_Manager();
    Resource_Manager(const Resource_Manager&);
    Resource_Manager& operator= (const Resource_Manager&);

public:

    /// Get the translated program name
    static QString program_name();
    
};

#endif // RESOURCE_MANAGER_HPP
