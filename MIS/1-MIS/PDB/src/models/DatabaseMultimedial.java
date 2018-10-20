// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package models;

import oracle.jdbc.internal.OraclePreparedStatement;
import oracle.jdbc.internal.OracleResultSet;
import oracle.ord.im.OrdImage;

import java.io.IOException;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Vector;

import static oracle.ord.im.OrdImage.*;

/**
 * @author xstane33
 * @author xskota07
 */
public class DatabaseMultimedial {
    /**
     *  Save photo to DB
     *  @param imagePath path to image
     *  @param resID ID of corresponding reservation
     *  @throws Exception if failed
     */
    public static void insertPhoto(String imagePath, Integer resID) throws Exception {
        Connection connection = Database.sharedInstance().getConnection();
        connection.setAutoCommit(false);

        Statement stmt = connection.createStatement();
        try {

            String SQLquery;
            OracleResultSet rset;
            SQLquery = ("INSERT INTO photo (reservation, img)" +
                    "VALUES (" + resID + ", ORDSYS.ORDIMAGE.INIT())");


            stmt.execute(SQLquery);
            //System.out.print(SQLquery + "\n");
            SQLquery = ("SELECT img FROM photo WHERE reservation = "
                    + resID + " FOR UPDATE");

            rset = (OracleResultSet) stmt.executeQuery(SQLquery);
            rset.next();

            //System.out.print(SQLquery + "\n");
            OrdImage imageProxy;
            imageProxy = (OrdImage) rset.getORAData("img", getORADataFactory());
            rset.close();

            //System.out.print("oradata" + "\n");
            imageProxy.loadDataFromFile(imagePath);
            imageProxy.setProperties();

            //System.out.print("load" + "\n");
            SQLquery = ("UPDATE photo SET img=? WHERE reservation = " + resID);

            //
            // after adding photo to database we must update all StillImage settings
            //

            OraclePreparedStatement preparedStmt;
            preparedStmt = (OraclePreparedStatement) connection.prepareStatement(SQLquery);
            preparedStmt.setORAData(1, imageProxy);
            preparedStmt.executeUpdate();
            preparedStmt.close();

            //System.out.print(SQLquery + "\n");
            SQLquery = ("UPDATE photo p SET p.img_si = SI_StillImage" +
                    "(p.img.getContent()) WHERE reservation = " +  resID);


            stmt = connection.createStatement();
            stmt.executeUpdate(SQLquery);

            SQLquery = ("UPDATE photo SET " +
                    "photo.img_ac = SI_AverageColor(photo.img_si), " +
                    "photo.img_ch = SI_ColorHistogram(photo.img_si), " +
                    "photo.img_pc = SI_PositionalColor(photo.img_si), " +
                    "photo.img_tx = SI_Texture(photo.img_si) WHERE reservation = " + resID);

            stmt.executeUpdate(SQLquery);
            stmt.close();

            //System.out.print(SQLquery + "\n");
            SQLquery = ("SELECT img FROM photo WHERE reservation = " + resID);

            stmt = connection.createStatement();
            stmt.executeQuery(SQLquery);

            //System.out.print(SQLquery + "\n");
            rset = (OracleResultSet) stmt.executeQuery(SQLquery);
            rset.next();

            //System.out.print("pred commitem");
            connection.commit();
        } catch (SQLException ex1) {
            System.out.printf("SQL error in inserting photo\n");
            System.out.printf(ex1.toString());
        } catch (IOException ex) {
            System.out.printf("IO error in inserting photo\n");
        }
        finally {
            stmt.close();
        }
    }
    /**
     *  Get image of a reservation
     *  @param resID ID of corresponding reservation
     *  @return image (OrdImage)
     *  @throws Exception if failed
     */
    public static OrdImage getPhoto (int resID) throws Exception {
        Connection connection = Database.sharedInstance().getConnection();
        connection.setAutoCommit(false);

        Statement stmt = connection.createStatement();
        try {
            String SQLquery;

            SQLquery = ("SELECT img FROM photo WHERE reservation = " + resID + "");
            //System.out.print(SQLquery);
            OracleResultSet rset = (OracleResultSet) stmt.executeQuery(SQLquery);
            rset.next();

            OrdImage imgProxy = (OrdImage) rset.getORAData("img", getORADataFactory());
            rset.close();
            stmt.close();

            connection.commit();
            return imgProxy;
        } catch (Exception ex2) {
            System.out.printf(ex2.toString());
        }
        return null;
    }
    /**
     *  Rotate a photo in DB
     *  @param resID ID of corresponding reservation
     *  @param rotation rotation angle (clockwise)
     *  @param xFlip flip over X axis flag
     *  @param yFlip flip over Y axis flag
     *  @throws Exception if failed
     */
    public static void rotatePhoto(int resID, int rotation, boolean xFlip, boolean yFlip) throws Exception {
        Connection connection = Database.sharedInstance().getConnection();
        connection.setAutoCommit(false);

        try {
            String SQLquery;
            Statement stmt = connection.createStatement();
            SQLquery = "SELECT img FROM photo WHERE reservation = " + resID + " FOR UPDATE";
            OracleResultSet rset = (OracleResultSet) stmt.executeQuery(SQLquery);
            rset.next();

            OrdImage imgProxy = (OrdImage) rset.getORAData("img", getORADataFactory());
            rset.close();

            if (rotation != 0)
                imgProxy.processCopy("rotate="+Integer.toString(rotation), imgProxy);
            if (xFlip == true)
                imgProxy.processCopy("flip", imgProxy);
            if (yFlip == true)
                imgProxy.processCopy("mirror", imgProxy);


            SQLquery = "UPDATE photo SET img=? WHERE reservation = " + resID;
            OraclePreparedStatement preparedStmt = (OraclePreparedStatement) connection.prepareStatement(SQLquery);
            preparedStmt.setORAData(1, imgProxy);
            preparedStmt.executeUpdate();
            preparedStmt.close();

            SQLquery = ("UPDATE photo p SET p.img_si = SI_StillImage" +
                    "(p.img.getContent()) WHERE p.reservation = " + resID);

            stmt = connection.createStatement();
            stmt.executeUpdate(SQLquery);

            SQLquery = ("UPDATE photo SET " +
                    "photo.img_ac = SI_AverageColor(photo.img_si), " +
                    "photo.img_ch = SI_ColorHistogram(photo.img_si), " +
                    "photo.img_pc = SI_PositionalColor(photo.img_si), " +
                    "photo.img_tx = SI_Texture(photo.img_si) WHERE reservation = " + resID);

            stmt.executeUpdate(SQLquery);
            stmt.close();


            SQLquery = ("SELECT img FROM photo WHERE reservation = " + resID);

            stmt = connection.createStatement();
            rset = (OracleResultSet) stmt.executeQuery(SQLquery);
            rset.next();

            connection.commit();
        } catch (Exception ex2) {
            System.out.printf(ex2.toString());
        }
    }
    /**
     *  Change scale of a photo in DB
     *  @param resID ID of corresponding reservation
     *  @param scaleFactor scale factor (1 is base)
     *  @throws Exception if failed
     */
    public static void scalePhoto(int resID, double scaleFactor) throws Exception {
        Connection connection = Database.sharedInstance().getConnection();
        connection.setAutoCommit(false);

        try {
            String SQLquery;
            Statement stmt = connection.createStatement();
            SQLquery = "SELECT img FROM photo WHERE reservation = " + resID + " FOR UPDATE";
            OracleResultSet rset = (OracleResultSet) stmt.executeQuery(SQLquery);
            rset.next();

            OrdImage imgProxy = (OrdImage) rset.getORAData("img", getORADataFactory());
            rset.close();

            if (scaleFactor != 1)
                imgProxy.processCopy("scale=" + Double.toString(scaleFactor), imgProxy);


            SQLquery = "UPDATE photo SET img=? WHERE reservation = " + resID;
            OraclePreparedStatement preparedStmt = (OraclePreparedStatement) connection.prepareStatement(SQLquery);
            preparedStmt.setORAData(1, imgProxy);
            preparedStmt.executeUpdate();
            preparedStmt.close();

            SQLquery = ("UPDATE photo p SET p.img_si = SI_StillImage" +
                    "(p.img.getContent()) WHERE p.reservation = " + resID);

            stmt = connection.createStatement();
            stmt.executeUpdate(SQLquery);

            SQLquery = ("UPDATE photo SET " +
                    "photo.img_ac = SI_AverageColor(photo.img_si), " +
                    "photo.img_ch = SI_ColorHistogram(photo.img_si), " +
                    "photo.img_pc = SI_PositionalColor(photo.img_si), " +
                    "photo.img_tx = SI_Texture(photo.img_si) WHERE reservation = " + resID);

            stmt.executeUpdate(SQLquery);
            stmt.close();


            SQLquery = ("SELECT img FROM photo WHERE reservation = " + resID);

            stmt = connection.createStatement();
            rset = (OracleResultSet) stmt.executeQuery(SQLquery);
            rset.next();


            connection.commit();
        } catch (Exception ex2) {
            System.out.printf(ex2.toString());
        }
    }
    /**
     *  Delete a photo in DB
     *  @param resID ID of corresponding reservation
     *  @throws Exception if failed
     */
    public static void deletePhoto(int resID) throws Exception {
        Connection connection = Database.sharedInstance().getConnection();
        connection.setAutoCommit(false);

        Statement stmt = connection.createStatement();

        try {
            String SQLquery;

            SQLquery = "DELETE FROM photo WHERE reservation = " + resID;
            stmt.executeQuery(SQLquery);
            connection.commit();
        } catch (SQLException ex1) {
            System.out.printf("SQL chyba\n");

            System.out.printf(ex1.toString());
        }  catch (Exception ex1) {
            System.out.printf("Jina chyba\n");
        }
    }

    /**
     *  Get most similar image
     *  @param resID ID of corresponding reservation
     *  @return image (OrdImage)
     *  @throws Exception if failed
     */
    public static OrdImage getMostSimilarImage (int resID) throws Exception {
        Connection connection = Database.sharedInstance().getConnection();
        connection.setAutoCommit(false);
        Statement stmt = connection.createStatement();

        Vector<Integer> idVector = new Vector<>();
        try {
            String SQLquery;

            //Prevzato ze slidu
            SQLquery = ("SELECT src.reservation as source, dst.reservation as dest, "
                    + " SI_ScoreByFtrList(new SI_FeatureList(src.img_ac, 0.3, src.img_ch, 0.3, src.img_pc, 0.1, src.img_tx, 0.3), dst.img_si) as similarity "
                    + " FROM photo src, photo dst "
                    + " WHERE src.reservation <> dst.reservation AND src.reservation = " + resID
                    + " ORDER BY similarity ASC");


            OracleResultSet rset = (OracleResultSet) stmt.executeQuery(SQLquery);

            while (rset.next()) {
                idVector.add(rset.getInt("dest"));
            }
            rset.close();
            stmt.close();

            connection.commit();
        } catch (Exception ex2) {
            System.out.printf(ex2.toString());
        }
        //
        if (idVector.isEmpty())
        {
            return null;
        }
        return getPhoto(idVector.firstElement());
    }
}
