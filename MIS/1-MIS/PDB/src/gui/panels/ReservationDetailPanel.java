package gui.panels;

import com.toedter.calendar.JDateChooser;
import gui.dialogs.PhotoEditDialog;
import models.Database;
import models.Reservation;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.ActionListener;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Statement;
import java.text.SimpleDateFormat;
/**
 * @author xstane33
 * @author xskota07
 * @author xklima22
*/
public class ReservationDetailPanel extends JPanel {

    private JPanel editPane;
    private JPanel showDetailPane;

    private JLabel customerLab;
    private JLabel dateFromLab;
    private JLabel dateToLab;
    private JLabel dateFromEditLab;

    private GridBagConstraints gbc;


    private JComboBox<String> customerBox;
    public JDateChooser dateTo;
    private JButton photoButton;


    private LayoutManagerPanel parent;
    private int selectedReservationId;

    public ReservationDetailPanel(LayoutManagerPanel parent) {
        this.setBackground(Color.red);

        this.setLayout(new GridBagLayout());
        this.gbc = new GridBagConstraints();
        this.parent = parent;
        this.selectedReservationId = -1;

        this.customerLab = new JLabel("\t");
        this.dateFromLab = new JLabel("\t");
        this.dateToLab = new JLabel("\t");
        this.dateFromEditLab = new JLabel("\t");

        this.photoButton = new JButton("Photo");
        this.photoButton.setVisible(false);
        this.photoButton.addActionListener(new ActionListener(){
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                //getFilteredReservations(new java.sql.Date(calendarDate.getDate().getTime()), new java.sql.Date(dateTo.getDate().getTime()));
                System.err.println("Show me the map!");
                PhotoEditDialog ped = new PhotoEditDialog();
                System.out.format("id  rezerevace je: %s\n", selectedReservationId);
                ped.createPhotoWinow(selectedReservationId);
            }
        });

        this.setEditPane();
        this.setShowDetailPane();
        //this.dateFrom.setDateFormatString("DD-MM-YYYY");
        //this.dateTo.setDateFormatString("DD-MM-YYYY");



        this.showInsertOptions(false);


        //this.editPane.setLayout(new BoxLayout(this.editPane, BoxLayout.Y_AXIS));
        this.gbc.anchor = GridBagConstraints.NORTHWEST;
        gbc.fill = GridBagConstraints.NONE;
        this.gbc.gridx      = 0;
        this.gbc.gridy      = 0;
        this.gbc.weightx    = 0;
        this.gbc.ipady      = 5;
        this.gbc.ipady      = 5;
        this.add(this.editPane, gbc);
        this.add(showDetailPane, gbc);
    }



    private JButton editButton;
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
                    customerBox.addItem(name);
                }

            }
        }
        catch(Exception ex)
        {
            System.err.format("SQLError while getting customers: %s\n", ex.getMessage());
        }
    }

    String getNameForCustomer(int id) {
        ResultSet rs;
        try {
            if(Database.sharedInstance().isConnected()) {
                Connection conn = Database.sharedInstance().getConnection();
                PreparedStatement ps = conn.prepareStatement("SELECT * FROM customer WHERE id = ?");
                ps.setInt(1, id);

                rs = ps.executeQuery();

                while(rs.next()) {
                    return rs.getString("name");
                }
            }
        }
        catch(Exception ex) {
            System.err.format("SQLError while getting customers: %s\n", ex.getMessage());
        }
        return "";
    }

    public void showInsertOptions(Boolean show) {
        if(show){
            this.dateFromEditLab.setText(this.parent.calendarDate.getDate().toString());
        }
        this.editPane.setVisible(show);

        this.showDetailPane.setVisible(!show);
        this.photoButton.setVisible(true);
        this.repaint();
    }


    private void setEditPane() {

        this.editPane = new JPanel(new GridLayout(7, 1));

        this.customerBox = new JComboBox<>();
        this.loadCustomersFromDB();
        this.dateTo = new JDateChooser();
        this.dateTo.setDate(this.parent.calendarDate.getDate());

        this.editPane.add(new JLabel("Customer: "));
        this.editPane.add(customerBox);

        this.editPane.add(new JLabel("Date From: "));
        java.sql.Date sqlDateFrom = new java.sql.Date(this.parent.calendarDate.getDate().getTime());
        this.dateFromEditLab.setText(sqlDateFrom.toString());
        this.editPane.add(this.dateFromEditLab);

        this.editPane.add(new JLabel("Date To: "));
        this.editPane.add(dateTo);

    }
    private void setShowDetailPane()
    {
        GridBagConstraints gbc = new GridBagConstraints();
        setLayout(new GridBagLayout());

        JPanel customerPanel = new JPanel(new GridLayout(1, 1));
        JPanel fromDatePanel = new JPanel(new GridLayout(1, 1));
        JPanel toDatePanel = new JPanel(new GridLayout(1, 1));
        JPanel photoPanel = new JPanel(new GridLayout(1, 1));


        Border customerBorder = BorderFactory.createTitledBorder("Customer: ");
        customerPanel.setBorder(customerBorder);

        Border fromDateBorder = BorderFactory.createTitledBorder("From Date: ");
        fromDatePanel.setBorder(fromDateBorder);

        Border toDateBorder = BorderFactory.createTitledBorder("To Date: ");
        toDatePanel.setBorder(toDateBorder);


        customerPanel.add(customerLab);
        fromDatePanel.add(dateFromLab);
        toDatePanel.add(dateToLab);
        photoPanel.add(photoButton);

        this.showDetailPane = new JPanel();

        this.showDetailPane.setLayout(new BoxLayout(this.showDetailPane, BoxLayout.Y_AXIS));

        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.ipadx = 10;
        this.showDetailPane.add(customerPanel, gbc);
        gbc.gridy++;
        this.showDetailPane.add(fromDatePanel, gbc);
        gbc.gridy++;
        this.showDetailPane.add(toDatePanel, gbc);
        gbc.gridy++;
        this.showDetailPane.add(photoPanel, gbc);
//
//        showDetailPane.add(customerPanel);
//        showDetailPane.add(fromDatePanel);
//        showDetailPane.add(toDatePanel);
//        showDetailPane.add(photoButton);


    }

    public void updateReservationDetailPanel(Reservation reservation)
    {
        SimpleDateFormat myDateFormat = new SimpleDateFormat("dd.MM.yyyy");

        this.photoButton.setEnabled(true);
        int id = reservation.getCustomerId();
        String name = this.getNameForCustomer(id);
        this.customerLab.setText(name);
        this.dateFromLab.setText(myDateFormat.format(reservation.getFromDate()));
        this.dateToLab.setText(myDateFormat.format(reservation.getToDate()));
        this.selectedReservationId = reservation.getId();

        this.repaint();
    }

    public void showNothing()
    {
        this.photoButton.setEnabled(false);
        this.customerLab.setText("\t");
        this.dateToLab.setText("\t");
        this.dateFromLab.setText("\t");

        this.repaint();
    }


}
