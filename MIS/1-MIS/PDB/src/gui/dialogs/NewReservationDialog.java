// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package gui.dialogs;

import com.toedter.calendar.JDateChooser;
import gui.panels.LayoutManagerMapPanel;
import models.Database;
import models.Reservation;

import javax.swing.*;
import javax.swing.border.Border;

import java.awt.*;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Statement;
import java.text.SimpleDateFormat;
import java.util.Date;


/**
 * @author xstane33
 * @author xskota07
 */
public class NewReservationDialog extends JPanel implements ItemListener {

    public JTextField aliasTextField;
    private JComboBox<String> customerComboBox;
    private ButtonGroup shapeButtonGroup;
    private JLabel fromDate;
    public JDateChooser toDate;
    private JRadioButton rectRadioButton;
    private JRadioButton circleRadioButton;
    private JRadioButton polygonRadioButton;

    private LayoutManagerMapPanel mapPanel;
    public Reservation reservation;

    /**
     * Class constructor
     * @param mapPanel parent
     */
    public NewReservationDialog(LayoutManagerMapPanel mapPanel) {

        this.mapPanel = mapPanel;
        this.reservation = new Reservation();
        GridBagConstraints gbc = new GridBagConstraints();
        setLayout(new GridBagLayout());

        JPanel mainPanel = new JPanel();

        JPanel aliasPanel = new JPanel(new GridLayout(1, 1));
        JPanel customerPanel = new JPanel(new GridLayout(1, 1));
        JPanel shapePanel = new JPanel(new GridLayout(1, 3));
        JPanel datePanel = new JPanel(new GridLayout(2, 1));



        Border shapeBorder = BorderFactory.createTitledBorder("Select shape: ");
        shapePanel.setBorder(shapeBorder);

        Border aliasBorder = BorderFactory.createTitledBorder("Alias: ");
        aliasPanel.setBorder(aliasBorder);

        Border customerBorder = BorderFactory.createTitledBorder("Customer: ");
        customerPanel.setBorder(customerBorder);

        Border dateBorder = BorderFactory.createTitledBorder("Set Dates: ");
        datePanel.setBorder(dateBorder);


        this.rectRadioButton = new JRadioButton("Rectangle");
        this.circleRadioButton = new JRadioButton("Circle");
        this.polygonRadioButton = new JRadioButton("Polygon");

        this.rectRadioButton.addItemListener(this);
        this.circleRadioButton.addItemListener(this);
        this.polygonRadioButton.addItemListener(this);


        //Group the radio buttons.
        this.shapeButtonGroup = new ButtonGroup();
        this.shapeButtonGroup.add(rectRadioButton);
        this.shapeButtonGroup.add(circleRadioButton);
        this.shapeButtonGroup.add(polygonRadioButton);


        this.aliasTextField = new JTextField();
        aliasPanel.add(aliasTextField);

        //INIT CUSTOMERU
        this.customerComboBox = new JComboBox<String>();
        customerPanel.add(customerComboBox);
        this.loadCustomersFromDB();

        //INIT DATUMU
        SimpleDateFormat myDateFormat = new SimpleDateFormat("dd.MM.yyyy");
        java.sql.Date sqlDateFrom = new java.sql.Date(this.mapPanel.parent.calendarDate.getDate().getTime());
        this.fromDate = new JLabel(myDateFormat.format(sqlDateFrom));
        this.reservation.setFromDate(sqlDateFrom);

        this.toDate = new JDateChooser();

        datePanel.add(new JLabel("From: "));
        datePanel.add(fromDate);
        datePanel.add(new JLabel("To: "));
        datePanel.add(toDate);

        shapePanel.add(rectRadioButton);
        shapePanel.add(circleRadioButton);
        shapePanel.add(polygonRadioButton);

        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));

        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.ipadx = 10;
        mainPanel.add(aliasPanel);
        gbc.gridy++;
        mainPanel.add(customerPanel);
        gbc.gridy++;
        mainPanel.add(datePanel);
        gbc.gridy++;
        mainPanel.add(shapePanel);

        this.add(mainPanel);
        Date date = new Date();
        toDate.setDate(this.mapPanel.parent.calendarDate.getDate());
        this.rectRadioButton.setSelected(true);


    }

    /**
     * Set values before showing the dialog
     * @param reservation reservation
     */
    public void presetReservation(Reservation reservation)
    {
        this.reservation.setCustomerId(reservation.getCustomerId());
        this.reservation.setFromDate(reservation.getFromDate());
        this.reservation.setToDate(reservation.getToDate());

        this.customerComboBox.setSelectedItem(this.reservation.getCustomerId());

        this.aliasTextField.setText(mapPanel.getLastSelection().getName());
        SimpleDateFormat myDateFormat = new SimpleDateFormat("dd.MM.yyyy");
        java.sql.Date sqlDateFrom = new java.sql.Date(this.reservation.getFromDate().getTime());
        this.fromDate = new JLabel(myDateFormat.format(sqlDateFrom));

        this.toDate.setDate(this.reservation.getToDate());
        this.rectRadioButton.setEnabled(false);
        this.circleRadioButton.setEnabled(false);
        this.polygonRadioButton.setEnabled(false);
    }
    /**
     * Loads all customers form DB
     */
    void loadCustomersFromDB(){
        try
        {
            if(Database.sharedInstance().isConnected())
            {
                Connection conn = Database.sharedInstance().getConnection();
                Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT * FROM customer");
                while(rs.next())
                {
                    String name = rs.getString("name");
                    customerComboBox.addItem(name);
                }
            }

        }
        catch(Exception ex)
        {
            System.err.format("SQLError while getting customers: %s\n", ex.getMessage());
        }
    }
    /**
     * Get ID of seleced customer
     * @return ID of selected customer
     */
    public int getIdforSelectedCustomer() {
        ResultSet rs;
        String name = String.valueOf(customerComboBox.getSelectedItem());
        try {
            if(Database.sharedInstance().isConnected()) {
                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement ps = conn.prepareStatement("SELECT * FROM customer WHERE name = ?");
                ps.setString(1, name);

                rs = ps.executeQuery();
                while(rs.next()) {
                    int i = rs.getInt("id");
                    return rs.getInt("id");
                }
            }
        }
        catch(Exception ex) {
            System.err.format("SQLError while getting customers: %s\n", ex.getMessage());
        }
        return -1;
    }
    /**
     * Item state changed
     */
    @Override
    public void itemStateChanged(ItemEvent e)
    {
        JRadioButton item = (JRadioButton) e.getItem();
        if(item.isSelected())
        {
            this.mapPanel.setSelectedShapeType(this.mapPanel.parent.getSelectedShapeByButtonName(item.getText()));

        }
    }

}
