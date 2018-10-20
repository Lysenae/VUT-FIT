

package models;

import enums.ShapeState;
import enums.ShapeType;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import oracle.spatial.geometry.JGeometry;
import spatial.shapes.ShapeBase;
import spatial.shapes.ShapeCircle;
import spatial.shapes.ShapeComposite;
import spatial.shapes.ShapeLine;
import spatial.shapes.ShapeLinePg;
import spatial.shapes.ShapePoint;
import spatial.shapes.ShapeRectangle;
import java.util.Date;
import java.text.Format;
import java.text.SimpleDateFormat;
import javax.swing.JOptionPane;

/**
 * Underlying model for LayoutManagerPanel and LayoutManagerMapPanel
 * @author xstane33
 * @author xskota07
 * @author xklima22
 */
public class LayoutManagerModel
{
    private static ArrayList<ShapePoint>     sPoints;
    private static ArrayList<ShapeLine>      sLines;
    private static ArrayList<ShapeRectangle> sRects;
    private static ArrayList<ShapeCircle>    sCircles;
    private static ArrayList<ShapeLinePg>    sPolygons;
    private static ArrayList<ShapeComposite> sComps;
    private static ArrayList<Reservation>    reservations;

    /**
     * Initialize lists.
     */
    public static void init()
    {
        sPoints      = new ArrayList<>();
        sLines       = new ArrayList<>();
        sRects       = new ArrayList<>();
        sCircles     = new ArrayList<>();
        sPolygons    = new ArrayList<>();
        sComps       = new ArrayList<>();
        reservations = new ArrayList<>();
    }

    /**
     * Clear all shape lists.
     */
    public static void clearAll()
    {
        sPoints.clear();
        sLines.clear();
        sRects.clear();
        sCircles.clear();
        sPolygons.clear();
        sComps.clear();
    }

    /**
     * Save shape to DB.
     * @param shape saved shape
     */
    public static void saveShape(ShapeBase shape)
    {
        try
        {
            if(Database.sharedInstance().isConnected())
            {
                int type = getShapeTypeIndex(shape.getType());
                int valid;
                if(type < 3 || shape.getId() == -1) valid =1;
                else valid =0;

                for(Reservation reservation: reservations)
                {
                    if(reservation.getAreaId() == shape.getId())
                        valid = 1;
                }

                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement ps = conn.prepareStatement(
                    "INSERT INTO area(name, pt_type, geometry, valid) VALUES (?, ?, ?, ?)");
                java.sql.Struct obj = JGeometry.storeJS(shape.toJGeometry(), conn);
                ps.setString(1, shape.getName());
                ps.setInt(2, type);
                ps.setObject(3, obj);
                ps.setInt(4, valid);
                int status = ps.executeUpdate();
                if(status > 0)
                {
                    shape.setState(ShapeState.OK);
                }

                if(type > 2)
                {

                    PreparedStatement stmt = conn.prepareStatement(
                            "SELECT * FROM area WHERE name=?");
                    stmt.setString(1, shape.getName());
                    ResultSet rs = stmt.executeQuery();

                    while(rs.next())
                    {
                        shape.reservation.setAreaId(rs.getInt("id"));
                        LayoutManagerModel.saveReservation(shape.reservation);
                    }
                }
            }
            else
            {
                JOptionPane.showMessageDialog(null,
                "InsertShape: Could not connect",
                "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                "InsertShape: Could not save shape " + shape.getName() + "\n" + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
        }
    }
    
    /**
     * Save reservation to DB.
     * @param reservation saved reservation
     */
    public static void saveReservation(Reservation reservation)
    {
        try 
        {
            if(Database.sharedInstance().isConnected())
            {
                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement ps = conn.prepareStatement(
                    "INSERT INTO reservation(customer, area, date_from, date_to) VALUES (?, ?, ?,?)");
                ps.setInt(1, reservation.getCustomerId());
                ps.setInt(2, reservation.getAreaId());
                ps.setDate(3, reservation.getFromDate());
                ps.setDate(4, reservation.getToDate());
                ps.executeUpdate();
            }
            else
            {
                JOptionPane.showMessageDialog(null,
                "SaveReservation: Could not connect",
                "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                "Insert: Could not save reservation " + reservation.getId() + "\n" + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    /**
     * Update shape geometry in DB.
     * @param shape updated shape
     */
    public static void updateShape(ShapeBase shape)
    {
        try
        {
            if(Database.sharedInstance().isConnected())
            {
                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement ps = conn.prepareStatement(
                        "UPDATE area SET geometry=? WHERE id=?");
                java.sql.Struct obj = JGeometry.storeJS(shape.toJGeometry(), conn);
                ps.setObject(1, obj);
                ps.setInt(2, shape.getId());
                int status = ps.executeUpdate();
                if(status > 0)
                {
                    shape.setState(ShapeState.OK);
                }
            }
            else
            {
                JOptionPane.showMessageDialog(null,
                "UpdateShape: Could not connect",
                "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                "Insert: Could not update shape " + shape.getName() + "\n" + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
        }
    }
    
    /**
     * Update shape info in DB.
     * @param shape updated shape
     */
    public static void updateShapeInfo(ShapeBase shape)
    {
        try
        {
            if(Database.sharedInstance().isConnected())
            {
                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement ps = conn.prepareStatement(
                        "UPDATE area SET name=? WHERE id=?");
                ps.setString(1, shape.getName());
                ps.setInt(2, shape.getId());
                int status = ps.executeUpdate();
                if(status > 0)
                {
                    shape.setState(ShapeState.OK);
                }

                PreparedStatement stmt = conn.prepareStatement(
                        "UPDATE reservation SET customer = ? , date_from = ?, date_to = ?  WHERE area=?");
                stmt.setInt(1, shape.reservation.getCustomerId());
                stmt.setDate(2, shape.reservation.getFromDate());
                stmt.setDate(3, shape.reservation.getToDate());
                stmt.setInt(4, shape.getId());
                stmt.executeUpdate();
            }
            else
            {
                JOptionPane.showMessageDialog(null,
                "UpdateShapeInfo: Could not connect",
                "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                "UpdateShapeInfo: Could not update shape " + shape.getName() + "\n" + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    /**
     * Delete shape from DB. Deleting only sets validity flag to 0. Shapes with
     * 0 validity flags are not loaded.
     * @param shape deleted shape
     */
    public static void deleteShapeFromDb(ShapeBase shape)
    {
        try
        {
            if(Database.sharedInstance().isConnected())
            {
                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement ps = conn.prepareStatement(
                        "UPDATE area SET valid=0 WHERE id=?");
                ps.setInt(1, shape.getId());
                ps.executeUpdate();
            }
            else
            {
                JOptionPane.showMessageDialog(null,
                "DeleteShapeInfo: Could not connect",
                "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                "UpdateShapeInfo: Could not delete shape " + shape.getName() + "\n" + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    /**
     * Load all valid shapes.
     * @return count of loaded shapes.
     */
    public static int loadAllShapes()
    {
        int count = 0;
        clearAll();
        try
        {
            if(Database.sharedInstance().isConnected())
            {
                Connection conn = Database.sharedInstance().getConnection();
                Statement stmt = conn.createStatement();

                PreparedStatement setinValid = conn.prepareStatement(
                        "UPDATE area SET valid = 0 WHERE pt_type > 2");
                setinValid.executeUpdate();

                ResultSet rs = stmt.executeQuery("SELECT * FROM area");
                while(rs.next())
                {

                    int lineType = rs.getInt("pt_type");
                    if(lineType > 2) {
                        int id = rs.getInt("id");
                        for (Reservation reservation : reservations)
                        {
                            if (reservation.getAreaId() == id)
                            {

                                PreparedStatement setValid = conn.prepareStatement(
                                        "UPDATE area SET valid = 1 WHERE id = ?");
                                setValid.setInt(1,id);
                                setValid.executeUpdate();
                            }
                        }
                    }
                }
                rs = stmt.executeQuery("SELECT * FROM area");
                while(rs.next())
                {
                    String name = rs.getString("name");
                    int lineType = rs.getInt("pt_type");
                    int id = rs.getInt("id");
                    byte[] o = rs.getBytes("geometry");
                    int valid = rs.getInt("valid");
                    if(valid == 1)
                    {
                        if(lineType == 5) count++;
                        JGeometry geo = JGeometry.load(o);
                        addShape(id, name, lineType, geo);
                    }
                }
            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                "LoadShapes: Error while loading shapes:\n" + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
        }
        return count;
    }
    
    /**
     * Load all reservation valid in given date.
     * @param date selected date
     * @return count of loaded reservations
     */
    public static int loadAllReservations(Date date)
    {
        reservations.clear();
        java.sql.Date sqlDate = new java.sql.Date(date.getTime());
        Format formatter = new SimpleDateFormat("YYYY-MM-dd HH:mm:ss");
        String dateString = formatter.format(sqlDate);
        try
        {
            if(Database.sharedInstance().isConnected())
            {

                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement ps = conn.prepareStatement("SELECT * FROM reservation WHERE"+
                        " date_from <= TO_DATE('"+ dateString + "','YYYY-MM-dd HH24:MI:ss') AND" +
                        " date_to >= TO_DATE('"+ dateString + "','YYYY-MM-dd HH24:MI:ss')");


                ResultSet rs = ps.executeQuery();
                while(rs.next())
                {
                    int reservationId = rs.getInt("id");
                    int customerId = rs.getInt("customer");
                    int areaId = rs.getInt("area");
                    Date fromDate = rs.getDate("date_from");
                    Date toDate = rs.getDate("date_to");
                    java.sql.Date sqldFromDate = new java.sql.Date(fromDate.getTime());
                    java.sql.Date sqlToDate = new java.sql.Date(toDate.getTime());

                    Reservation reservation= new Reservation();


                    reservation.setId(reservationId);
                    reservation.setCustomerId(customerId);
                    reservation.setAreaId(areaId);
                    reservation.setFromDate(sqldFromDate);
                    reservation.setToDate(sqlToDate);
                    reservation.setInDatabase(true);

                    reservations.add(reservation);
                }

            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                "LoadReservations: Error while loading reservations:\n" + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
        }
        return reservations.size();
    }

    /**
     * Checks if modelis in consistent state, all loaded shapes are in the same
     * state as those in DB.
     * @return true if model is in consisten state
     */
    public static boolean isConsistent()
    {
        if (!sPoints.stream().noneMatch((s) -> (!s.isConsistent())))
        {
            return false;
        }

        if (!sLines.stream().noneMatch((s) -> (!s.isConsistent())))
        {
            return false;
        }

        if (!sCircles.stream().noneMatch((s) -> (!s.isConsistent())))
        {
            return false;
        }

        if (!sRects.stream().noneMatch((s) -> (!s.isConsistent())))
        {
            return false;
        }

        if (!sPolygons.stream().noneMatch((s) -> (!s.isConsistent())))
        {
            return false;
        }

        if (!sComps.stream().noneMatch((s) -> (!s.isConsistent())))
        {
            return false;
        }

        return true;
    }

    /**
     * Get points list.
     * @return point lists
     */
    public static ArrayList<ShapePoint> getPoints()
    {
        return sPoints;
    }

    /**
     * Get lines list.
     * @return lines list
     */
    public static ArrayList<ShapeLine> getLines()
    {
        return sLines;
    }

    /**
     * Get circles list.
     * @return circles list
     */
    public static ArrayList<ShapeCircle> getCircles()
    {
        return sCircles;
    }

    /**
     * Get rectangles list.
     * @return rectangles list
     */
    public static ArrayList<ShapeRectangle> getRects()
    {
        return sRects;
    }

    /**
     * Get polygons list.
     * @return polygons list
     */
    public static ArrayList<ShapeLinePg> getPolygons()
    {
        return sPolygons;
    }

    /**
     * Get composites list.
     * @return composites list
     */
    public static ArrayList<ShapeComposite> getComposites()
    {
        return sComps;
    }

    /**
     * Add point to list.
     * @param s point
     */
    public static void addPoint(ShapePoint s)
    {
        sPoints.add(s);
    }

    /**
     * Add line to list.
     * @param s line
     */
    public static void addLine(ShapeLine s)
    {
        sLines.add(s);
    }

    /**
     * Add circle to list.
     * @param s circle
     */
    public static void addCircle(ShapeCircle s)
    {
        LayoutManagerModel.getReservationForShape(s);
        sCircles.add(s);
    }

    /**
     * Add rectangle to list.
     * @param s rectangle
     */
    public static void addRect(ShapeRectangle s)
    {
        LayoutManagerModel.getReservationForShape(s);
        sRects.add(s);
    }
    
     /**
     * Add polygon to list.
     * @param s polygon
     */
    public static void addPolygon(ShapeLinePg s)
    {
        LayoutManagerModel.getReservationForShape(s);
        sPolygons.add(s);
    }

    /**
     * Add composite to list.
     * @param s composite
     */
    public static void addComposite(ShapeComposite s)
    {
        LayoutManagerModel.getReservationForShape(s);
        sComps.add(s);
    }

    /**
     * Save all shapes if needed.
     */
    public static void saveAll()
    {
        savePoints();
        saveLines();
        saveCircles();
        saveRects();
        savePolygons();
        saveComposites();
    }

    /**
     * Save points if needed.
     */
    public static void savePoints()
    {
        for(ShapePoint s : sPoints)
            callDbAction(s);
    }

    /**
     * Save lines if needed.
     */
    public static void saveLines()
    {
        for(ShapeLine s : sLines)
            callDbAction(s);
    }

    /**
     * Save circles if needed.
     */
    public static void saveCircles()
    {
        for(ShapeCircle s : sCircles)
            callDbAction(s);
    }

    /**
     * Save rectangles if needed.
     */
    public static void saveRects()
    {
        for(ShapeRectangle s : sRects)
            callDbAction(s);
    }

    /**
     * Save polygons if needed.
     */
    public static void savePolygons()
    {
        for(ShapeLinePg s : sPolygons)
            callDbAction(s);
    }

    /**
     * Save composites if needed.
     */
    public static void saveComposites()
    {
        for(ShapeComposite s : sComps)
            callDbAction(s);
    }
    
    /**
     * Gets reservation for given shape.
     * @param s shape
     */
    public static void getReservationForShape(ShapeBase s)
    {

        for (Reservation reservation : reservations) {
            if(reservation.getAreaId() == s.getId())
            {
                s.reservation.setId(reservation.getId());
                s.reservation.setCustomerId(reservation.getCustomerId());
                s.reservation.setFromDate(reservation.getFromDate());
                s.reservation.setToDate(reservation.getToDate());
                s.reservation.setInDatabase(reservation.isInDatabase());
                break;
            }
        }
    }

    /**
     * Delete shape from DB.
     * @param s shape
     */
    private static void deleteShapeBase(ShapeBase s)
    {
        if(s.getId() != -1)
        {

            Boolean set = false;
            int i = 0;
            for(Reservation reservation: reservations)
            {
                if(reservation.getAreaId() == s.getId())
                {
                    set = true;
                    break;
                }
                i++;
            }
            if(set)reservations.remove(i);
            try
            {
                if(Database.sharedInstance().isConnected())
                {
                    Connection conn = Database.sharedInstance().getConnection();
                    PreparedStatement stmt = conn.prepareStatement(
                            "DELETE FROM reservation WHERE area=?");
                    stmt.setInt(1, s.getId());
                    stmt.executeQuery();
                }

            }
            catch(Exception ex)
            {
                JOptionPane.showMessageDialog(null,
                "DeleteShape: Error while loading customers:\n" + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    /**
     * Remove shape from appropriate list.
     * @param s shape
     */
    public static void removeShapeFromList(ShapeBase s)
    {
        switch(s.getType())
        {
            case POINT:
                removePoint((ShapePoint)s);
                break;
            case LINE:
                removeLine((ShapeLine)s);
                break;
            case CIRCLE:
                removeCircle((ShapeCircle)s);
                break;
            case RECTANGLE:
                removeRect((ShapeRectangle)s);
                break;
            case POLYGON:
                removePolygon((ShapeLinePg)s);
                break;
            case COMPOSITE:
                removeComposite((ShapeComposite)s);
                break;
            default:
                break;
        }
        deleteShapeBase(s);
    }

    /**
     * Remove point from list.
     * @param s point
     */
    public static void removePoint(ShapePoint s)
    {
        if(s.isNew())
        {
            sPoints.remove(s);
        }
        else
        {
            s.setState(ShapeState.DELETED);
        }
    }

    /**
     * Remove line from list.
     * @param s line
     */
    public static void removeLine(ShapeLine s)
    {
        if(s.isNew())
        {
            sLines.remove(s);
        }
        else
        {
            s.setState(ShapeState.DELETED);
        }
    }

    /**
     * Remove circle from list.
     * @param s circle
     */
    public static void removeCircle(ShapeCircle s)
    {
        if(s.isNew())
        {
            sCircles.remove(s);
        }
        else
        {
            s.setState(ShapeState.DELETED);
        }
    }

    /**
     * Remove rectanlge from list.
     * @param s rectangle
     */
    public static void removeRect(ShapeRectangle s)
    {
        boolean staus;
        if(s.isNew())
        {
            sRects.remove(s);
        }
        else
        {
            s.setState(ShapeState.DELETED);
        }
    }

    /**
     * Remove polygon from list.
     * @param s polygon
     */
    public static void removePolygon(ShapeLinePg s)
    {
        if(s.isNew())
        {
            sPolygons.remove(s);
        }
        else
        {
            s.setState(ShapeState.DELETED);
        }
    }

    /**
     * Remove composite from list.
     * @param s composite
     */
    public static void removeComposite(ShapeComposite s)
    {
        if(s.isNew())
        {
            sComps.remove(s);
        }
        else
        {
            s.setState(ShapeState.DELETED);
        }
    }

    /**
     * Get area of shape.
     * @param s shape
     * @return area of shape
     */
    public static double getAreaofShape(ShapeBase s)
    {
        try
        {
            if(Database.sharedInstance().isConnected())
            {
                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement stmt = conn.prepareStatement(
                        "SELECT SDO_GEOM.SDO_AREA(geometry, 0.005) AS a FROM area WHERE id = ?");
                stmt.setInt(1, s.getId());
                ResultSet rs = stmt.executeQuery();
                if(rs.next())
                    return (double)rs.getFloat("a");
            }
            else
            {
                JOptionPane.showMessageDialog(null,
                "GetArea: Could not connect",
                "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                "GetArea: " + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
        }
        return Double.NaN;
    }

    /**
     * Get length of shape.
     * @param s shape
     * @return length of shape
     */
    public static double getLengthofShape(ShapeBase s)
    {
        try
        {
            if(Database.sharedInstance().isConnected())
            {
                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement stmt = conn.prepareStatement(
                        "SELECT SDO_GEOM.SDO_LENGTH(geometry, 0.005) AS a FROM area WHERE id = ?");
                stmt.setInt(1, s.getId());
                ResultSet rs = stmt.executeQuery();
                if(rs.next())
                    return (double)rs.getFloat("a");
            }
            else
            {
                JOptionPane.showMessageDialog(null,
                "GetLength: Could not connect",
                "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                "GetLength: " + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
        }
        return Double.NaN;
    }

    /**
     * Get biggest shape.
     * @return biggest shape or null
     */
    public static ShapeBase getBiggestShape()
    {
        try
        {
            if(Database.sharedInstance().isConnected())
            {
                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement ps = conn.prepareStatement(
                        "SELECT a.id, a.name name_b, a.valid, SDO_GEOM.SDO_AREA(a.geometry, 1) a_area " +
                                "FROM area a " +
                                "WHERE a.valid = 1 AND NOT EXISTS (" +
                                "   SELECT 1 FROM area b " +
                                "   WHERE b.valid = 1 AND (SDO_GEOM.SDO_AREA(b.geometry, 1) > " +
                                "       SDO_GEOM.SDO_AREA(a.geometry, 1)) AND " +
                                "       (a.id <> b.id))");
                ResultSet rs = ps.executeQuery();
                if(rs.next())
                    return getShapeById(rs.getInt("id"));
            }
            else
            {
                JOptionPane.showMessageDialog(null,
                    "GetBiggestShape: Could not connect",
                    "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                    "GetBiggestShape: " + ex.getMessage(),
                    "Error", JOptionPane.ERROR_MESSAGE);
        }
        return null;
    }

    /**
     * Get nearest shape to given shape.
     * @param shape shape for which nearest shape is searched
     * @return nearest shape or null
     */
    public static ShapeBase getNearestShape(ShapeBase shape)
    {
        try
        {
            if(Database.sharedInstance().isConnected())
            {
                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement ps = conn.prepareStatement(
                        "SELECT a.id, SDO_NN_DISTANCE(1) AS dist FROM area a " +
                                "WHERE a.valid = 1 AND SDO_NN(geometry, " +
                                "(SELECT o.geometry FROM area o " +
                                "WHERE o.id = ?), 'sdo_batch_size = 1', 1) = 'TRUE' AND " +
                                "ROWNUM <= ? AND id <> ? ORDER BY dist");
                ps.setInt(1, shape.getId());
                ps.setInt(2, 1);
                ps.setInt(3, shape.getId());
                ResultSet rs = ps.executeQuery();
                if(rs.next())
                {
                    return getShapeById(rs.getInt("id"));
                }
            }
            else
            {
                JOptionPane.showMessageDialog(null,
                    "GetNearestShape: Could not connect",
                    "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                "GetNearestShape: " + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
        }
        return null;
    }

    /**
     * Joins valid overlapping shapes into one.
     */
    public static void joinOverlappedShapes()
    {
        try
        {
            if(Database.sharedInstance().isConnected())
            {
                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement ps = conn.prepareStatement(
                        "SELECT DISTINCT a1.id id1, a2.id id2, a1.name n1, a2.name n2 " +
                                "FROM area a1, area a2 " +
                                "WHERE a1.id<>a2.id AND a1.valid=1 AND a2.valid=1 AND " +
                                "a1.pt_type > 2 AND a2.pt_type > 2 AND " +
                                "SDO_RELATE(a1.geometry, a2.geometry," +
                                "'MASK=OVERLAPBDYINTERSECT+CONTAINS+INSIDE') = 'TRUE' " +
                                "ORDER BY a1.id");
                while (true)
                {
                    ResultSet rs = ps.executeQuery();
                    if(rs.next())
                    {
                        int id1 = rs.getInt("id1");
                        int id2 = rs.getInt("id2");
                        String n1 = rs.getString("n1");
                        String n2 = rs.getString("n2");

                        String name = n1+"_"+n2;

                        PreparedStatement insertPs = conn.prepareStatement(
                                "INSERT INTO area(name, pt_type, geometry, valid) " +
                                        "VALUES (?, 6, SDO_GEOM.SDO_UNION((" +
                                        "SELECT geometry FROM area WHERE id=?)," +
                                        "(SELECT geometry FROM area WHERE id=?),0.005), 1)");
                        insertPs.setString(1, name);
                        insertPs.setInt(2, id1);
                        insertPs.setInt(3, id2);

                        PreparedStatement deletePs = conn.prepareStatement(
                                "UPDATE area SET valid = 0 WHERE id IN (?, ?)");

                        deletePs.setInt(1, id1);
                        deletePs.setInt(2, id2);

                        insertPs.executeUpdate();
                        deletePs.executeUpdate();

                        PreparedStatement getFirstReservation = conn.prepareStatement(
                                "SELECT * FROM reservation WHERE area = ?");
                        getFirstReservation.setInt(1,id1);

                        ResultSet rs2 = getFirstReservation.executeQuery();
                        int reservationId = -1;
                        while(rs2.next())
                        {
                            reservationId = rs2.getInt("id");
                        }

                        PreparedStatement stmt = conn.prepareStatement(
                                "SELECT * FROM area WHERE name=?");
                        stmt.setString(1, name);
                        ResultSet rs3 = stmt.executeQuery();

                        int newAreaId = -1;
                        while(rs3.next())
                        {
                            newAreaId = rs3.getInt("id");
                        }

                        PreparedStatement updateArea = conn.prepareStatement(
                                "UPDATE reservation SET area = ? WHERE id = ?");
                        updateArea.setInt(1,newAreaId);
                        updateArea.setInt(2, reservationId);
                        updateArea.executeUpdate();

                        PreparedStatement getSecondReservation = conn.prepareStatement(
                                "SELECT * FROM reservation WHERE area = ?");
                        getSecondReservation.setInt(1,id2);

                        ResultSet rs4 = getSecondReservation.executeQuery();
                        int reservationId2 = -1;
                        while(rs4.next())
                        {
                            reservationId2 = rs4.getInt("id");
                        }

                        PreparedStatement stmt2 = conn.prepareStatement(
                                "DELETE FROM reservation WHERE id=?");
                        stmt2.setInt(1, reservationId2);
                        stmt2.executeQuery();
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                JOptionPane.showMessageDialog(null,
                    "JoinOverlapped: Could not connect",
                    "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                "JoinOverlapped: " + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    /**
     * Checks if given shape is overlapped by point or line;s control point.
     * Always returns false for points and lines.
     * @param s shape
     * @return true if shape is overlapped by point.
     */
    public static boolean isOverlappedByMultipoint(ShapeBase s)
    {
        int i = 0;
        try
        {
            if(Database.sharedInstance().isConnected())
            {
                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement ps = conn.prepareStatement(
                        "SELECT DISTINCT a1.id id1, a2.id id2 " +
                                "FROM area a1, area a2 " +
                                "WHERE a1.id = ? AND a1.id<>a2.id AND " +
                                "a1.valid=1 AND a2.valid=1 AND " +
                                "a1.pt_type > 2 AND a2.pt_type <= 2 AND " +
                                "SDO_RELATE(a1.geometry, a2.geometry," +
                                "'MASK=OVERLAPBDYINTERSECT+CONTAINS+INSIDE') = 'TRUE' " +
                                "ORDER BY a1.id");
                ps.setInt(1, s.getId());
                ResultSet rs = ps.executeQuery();
                while(rs.next())
                {
                    ++i;
                }
            }
            else
            {
                JOptionPane.showMessageDialog(null,
                    "OverlappedByMp: Could not connect",
                    "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null,
                "OverlappedByMp: " + ex.getMessage(),
                "Error", JOptionPane.ERROR_MESSAGE);
        }
        System.out.println(i);
        return i > 0;
    }

    /**
     * Finds shape by ID.
     * @param id shape ID
     * @return shape with given ID, always returns null for shapes with 0 valid flag
     */
    private static ShapeBase getShapeById(int id)
    {
        if(id >= 0)
        {
            try
            {
                if(Database.sharedInstance().isConnected())
                {
                    Connection conn = Database.sharedInstance().getConnection();
                    PreparedStatement ps = conn.prepareStatement(
                            "SELECT * FROM area WHERE id = ?");
                    ps.setInt(1, id);
                    ResultSet rs = ps.executeQuery();
                    if(rs.next())
                    {
                        int sid = rs.getInt("id");
                        int lineType = rs.getInt("pt_type");
                        int valid = rs.getInt("valid");
                        if(valid == 1)
                        {
                            return findShape(sid, lineType);
                        }
                    }
                }
            }
            catch(Exception ex)
            {
                JOptionPane.showMessageDialog(null,
                    "GetShapeById: " + ex.getMessage(),
                    "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
        return null;
    }

    /**
     * Call appropriate DB action for shape - save, delete, update.
     * @param s shape
     */
    private static void callDbAction(ShapeBase s)
    {
        if(s.isNew())
        {
            LayoutManagerModel.saveShape(s);
        }
        else if(s.isUpdated())
        {
            LayoutManagerModel.updateShape(s);
        }
        else if(s.isDeleted())
        {
            LayoutManagerModel.deleteShapeFromDb(s);
        }
        else if(s.isInfoUpdated())
        {
            LayoutManagerModel.updateShapeInfo(s);
        }
    }

    /**
     * Gets shape type DB type. See init.sql.
     * @param st shape type
     * @return ID of shape type used in DB
     */
    private static int getShapeTypeIndex(ShapeType st)
    {
        switch (st)
        {
            case POINT:
                return 1;
            case LINE:
                return 2;
            case POLYGON:
                return 3;
            case RECTANGLE:
                return 4;
            case CIRCLE:
                return 5;
            default:
                return 6;
        }
    }

    /**
     * Add shape to appropriate list.
     * @param id shape id
     * @param name shape name
     * @param lineType shape DB type
     * @param geo JGeometry object
     */
    private static void addShape(int id, String name, int lineType, JGeometry geo)
    {
        switch (lineType)
        {
            case 1:
                addPoint(ShapePoint.fromJGeometry(id, name, geo));
                break;
            case 2:
                addLine(ShapeLine.fromJGeometry(id, name, geo));
                break;
            case 3:
                addPolygon(ShapeLinePg.fromJGeometry(id, name, geo));
                break;
            case 4:
                addRect(ShapeRectangle.fromJGeometry(id, name, geo));
                break;
            case 5:
                addCircle(ShapeCircle.fromJGeometry(id, name, geo));
                break;
            case 6:
                addComposite(new ShapeComposite(id, name, geo));
                break;
            default:
                break;
        }
    }

    /**
     * Finds shape type with given ID of given type.
     * @param id shape id
     * @param lineType shape DB type
     * @return ShapeBase
     */
    private static ShapeBase findShape(int id, int lineType)
    {
        switch (lineType)
        {
            case 1:
                for(ShapePoint s : sPoints)
                    if(s.getId() == id)
                        return s;
            case 2:
                for(ShapeLine s : sLines)
                    if(s.getId() == id)
                        return s;
            case 3:
                for(ShapeLinePg s : sPolygons)
                    if(s.getId() == id)
                        return s;
            case 4:
                for(ShapeRectangle s : sRects)
                    if(s.getId() == id)
                        return s;
            case 5:
                for(ShapeCircle s : sCircles)
                    if(s.getId() == id)
                        return s;
            case 6:
                for(ShapeComposite s : sComps)
                    if(s.getId() == id)
                        return s;
            default:
                return null;
        }
    }
}
