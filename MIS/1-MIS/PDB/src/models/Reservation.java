// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package models;

import spatial.shapes.ShapeBase;

import java.sql.Date;

/**
 * @author xskota07
 * @author xstane33
 */
public class Reservation {
    private int id;
    private Date dateFrom;
    private Date dateTo;
    private boolean inDatabase;
    private int areaId;
    private int customerId;

    /**
     * Constructor
     */
    public Reservation(){
        inDatabase = false;
    }

    /**
     * Get reservation ID
     * @return reservation ID
     */
    public int getId() {
        return id;
    }
    /**
     * Set reservation ID
     * @param id value
     */
    public void setId(int id) {
        this.id = id;
    }

    /**
     * Get customer ID
     * @return customer ID
     */
    public int getCustomerId() {
        return customerId;
    }
    /**
     * Set customer ID
     * @param id value
     */
    public void setCustomerId(int id) {
        this.customerId = id;
    }

    /**
     * Get area ID
     * @return area ID
     */
    public int getAreaId() {return areaId;}
    /**
     * Set area ID
     * @param id value
     */
    public void setAreaId(int id) {this.areaId = id;}

    /**
     * Check if the reservation is in database
     * @return true if true
     */
    public boolean isInDatabase() {
        return inDatabase;
    }
    /**
     * Set inDatabase flag
     * @param inDatabase flag
     */
    public void setInDatabase(boolean inDatabase) {
        this.inDatabase = inDatabase;
    }

    /**
     * Get beginning date of the reservation
     * @return beginning date of the reservation
     */
    public Date getFromDate() {return this.dateFrom;}
    /**
     * Set beginning date of the reservation
     * @param datef beginning date
     */
    public void setFromDate(Date datef) {this.dateFrom = datef;}

    /**
     * Get ending date of the reservation
     * @return ending date of the reservation
     */
    public Date getToDate() {return this.dateTo;}
    /**
     * Set end date of the reservation
     * @param datet end date
     */
    public void setToDate(Date datet) {this.dateTo = datet; }

    /**
     * Duplicate a reservation
     * @return the original one
     */
    public Reservation dup()
    {
        return fromData(this.customerId, this.dateFrom, this.dateTo);
    }
    /**
     * Create a reservation
     * @param customerId customer ID
     * @param from beginning date
     * @param to end date
     * @return new reservation
     */
    public static Reservation fromData(int customerId, Date from, Date to)
    {
        Reservation r = new Reservation();
        r.setCustomerId(customerId);
        r.setFromDate(from);
        r.setToDate(to);
        return r;
    }
}
