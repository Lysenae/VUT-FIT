// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package models;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import oracle.jdbc.pool.OracleDataSource;

/**
 * @author xskota07
 * @author xstane33
 * @author xklima22
 */
public class Database {
    // Shared instance
    private static Database database;

    // Oracle connection data
    private final OracleDataSource oracleDataSource;

    // Connetion to Oracle server
    private Connection connection;

    // Connection string to Berta server
    private final String connetionString = "jdbc:oracle:thin:@//berta.fit.vutbr.cz:1526/pdb1";
    
    private Database() throws SQLException {
        oracleDataSource = new OracleDataSource();
        oracleDataSource.setURL(connetionString);
        oracleDataSource.setUser("");
        oracleDataSource.setPassword("");
    }
    /**
     * Initialise database
     * @return database instance
     */
    public static Database sharedInstance() {
        try
        {
            if (database == null) {
                database = new Database();
            }
            return database;
        }
        catch(SQLException sqlEx)
        {
            System.err.println(sqlEx.getMessage());
        }
        return null;
    }

    /**
     * Set DB user
     *  @param user username
     *  @param password password
     */
    private void setUser(String user, String password) {
        oracleDataSource.setUser(user);
        oracleDataSource.setPassword(password);
    }

    /**
     *  Connect to DB
     *  @throws SQLException if failed
     */
    private void connect() throws SQLException {
        close(); // if connect, close old connection
        connection = oracleDataSource.getConnection();
        System.out.println("Connect");
        System.out.println(connection.isValid(5));
    }
    /**
     *  Connect an user to DB
     *  @param user username
     *  @param password password
     *  @throws SQLException if failed
     */
    public void connect(String user, String password) throws SQLException {
        setUser(user, password);
        connect();
    }
    /**
     *  Connect to DB
     *  @throws SQLException if failed
     */
    public void close() throws SQLException {
        if (connection != null) {
            connection.close();
        }
    }
    /**
     *  Close DB connection
     *  @throws SQLException if failed
     */
    public void disconnect() throws SQLException {
        close();
    }
    /**
     *  Connect an user to DB
     *  @return true of user is connected and connection is valid
     */
    public boolean isConnected() {
        try {
            if(connection != null && connection.isValid(10)) {
                return true;
            }
        }
        catch(Exception ex) {
            System.err.format("Database.isConnected(): %s", ex.getMessage());
        }
        return false;
    }
    /**
     *  Get connection
     *  @return connection instance
     */
    public Connection getConnection()
    {
        return this.connection;
    }

    /**
     *  run an SQL script
     *  @param fineName SQLQuery file
     *  @throws SQLException if failed
     */
    public void runSQLScript(String fineName) throws SQLException {

        // Load SQL script from file
        BufferedReader input = new BufferedReader(new InputStreamReader(this.getClass().getClassLoader().getResourceAsStream("sql/" + fineName)));

        String curentLine = "";
        String sqlQuery = "";
        boolean isTrigger = false;
        try {
            // Read line
            while ((curentLine = input.readLine()) != null) {
                // Skip empty lines and comments
                if (curentLine.length() == 0 || (curentLine.length() > 0 && curentLine.charAt(0) == '-')) {
                    continue;
                }

                // Add line to sql query
                sqlQuery += " " + curentLine;

                // Last char is ';' => SQL command is completed
                boolean isTrigger2 = sqlQuery.charAt(sqlQuery.length() - 1) == '/';
                if (sqlQuery.charAt(sqlQuery.length() - 1) == ';' || isTrigger2) {
                    
                    // Replace ';' to space
                    if(!isTrigger2) {
                        sqlQuery = sqlQuery.replace(';', ' ');
                    }
                    else {
                        sqlQuery = sqlQuery.replace('/', ' ');
                    }
                    
                    // Check if Query is DROP
                    boolean isDropQuery = false;
                    if((curentLine.charAt(0) == 'D'
                            && curentLine.charAt(1) == 'R'
                            && curentLine.charAt(2) == 'O'
                            && curentLine.charAt(3) == 'P') || 
                          (curentLine.charAt(0) == 'd'
                            && curentLine.charAt(1) == 'r'
                            && curentLine.charAt(2) == 'o'
                            && curentLine.charAt(3) == 'p')) {
                        isDropQuery = true;
                    }
                    if((curentLine.charAt(0) == 'c'
                            && curentLine.charAt(1) == 'r'
                            && curentLine.charAt(2) == 'e'
                            && curentLine.charAt(3) == 'a'
                            && curentLine.charAt(4) == 't'
                            && curentLine.charAt(5) == 'e'
                            && curentLine.charAt(6) == ' '
                            && curentLine.charAt(7) == 'o'
                            && curentLine.charAt(8) == 'r'
                            && curentLine.charAt(9) == ' '
                            && curentLine.charAt(10) == 'r'
                            && curentLine.charAt(11) == 'e'
                            && curentLine.charAt(12) == 'p')) 
                    {
                        isTrigger = true;
                    }
                                    
                    // Execute query
                    Statement stmt = Database.sharedInstance().getConnection().createStatement();
                    try {
                        stmt.executeUpdate(sqlQuery);
                    }
                    catch(SQLException ex) {
                        // Check if error when DROP TABLE / DROP INDEX whitch doesnt exist
                        if(isDropQuery && ex.getErrorCode() == 942 || ex.getErrorCode() == 1418) {
                            continue;
                        }
                        throw ex;
                    }
                    finally {
                        stmt.close();
                        // Clean SQL query
                        sqlQuery = "";
                    }
                }
            }
        }
        catch (IOException ex) {
            System.err.println("ERROR: IOException run script");
        }
    }
}
