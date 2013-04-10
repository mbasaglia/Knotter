#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <QObject>
#include <QDebug>

/**
 * Manage resources and data
 */
class Resource_Manager : public QObject
{
    Q_OBJECT

    Resource_Manager(){}
    Resource_Manager(const Resource_Manager&);
    Resource_Manager& operator= (const Resource_Manager&);

    static Resource_Manager singleton;

public:

    /// Initialize the resource system
    static void initialize();

    /// Get the translated program name
    static QString program_name();

    /// Get version as string
    static QString program_version();

    /// Check if has at least given version number
    static bool has_least_version(int maj, int min);


    /// Check if given version string has at least given version number
    static bool check_least_version(QString version, int maj, int min);


    /** Get full path of given data file
     * \param name Name of the file (relative to datadir)
     * \return Full path or null string if not found
     */
    static QString data(QString name);
    
};

#endif // RESOURCE_MANAGER_HPP
