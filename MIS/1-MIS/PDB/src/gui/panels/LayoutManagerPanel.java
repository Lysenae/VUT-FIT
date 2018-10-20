package gui.panels;

import com.toedter.calendar.JDateChooser;
import enums.LmMode;
import enums.ShapeType;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.Date;
import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.util.Calendar;

import models.LayoutManagerModel;
import spatial.shapes.ShapeBase;
import gui.dialogs.NewReservationDialog;
import helpers.DateHelper;
import listeners.LmpInsertButtonListener;
import listeners.LmpShowButtonListener;
import listeners.LmpUpdateButtonListener;

/**
 * @author xskota07
 * @author xklima22
 */
public class LayoutManagerPanel
        extends JPanel
        implements ItemListener, DocumentListener, ActionListener
{
    private GridBagConstraints gbc;
    private JScrollPane mapContainer;
    public LayoutManagerMapPanel mapPanel;
    public ReservationDetailPanel reservationDetailPanel;

    private boolean btnsEnabled;

    private JPanel toolPanel;
    private JPanel calendarPanel;

    private JPanel pRBtns;
    private JLabel lSelectMode;
    private ButtonGroup selectMode;

    private JButton insertPointButton;
    private JButton insertLineButton;
    private JRadioButton modeShapeMove;
    private JRadioButton modeShapeResize;
    private JRadioButton modeShapeSelect;

    private JPanel pBtns;
    private JButton delBtn;

    private JPanel pDetails;
    private JTable tDetails;

    private JButton showButton;
    private JButton insertButton;
    private JButton updateButton;
    private JButton saveButton;
    public JDateChooser calendarDate;
    public JLabel reservationsLabel;
    public JLabel stagesLabel;

    /**
     * Class constructor.
     */
    public LayoutManagerPanel()
    {
        this.reservationsLabel = new JLabel("Reservations: 0 ");
        this.stagesLabel = new JLabel(" Stages: 0 ");

        this.setLayout(new GridBagLayout());

        this.calendarDate = new JDateChooser(new Date());
        this.gbc          = new GridBagConstraints();
        this.toolPanel    = new JPanel();
        this.calendarPanel = new JPanel();
        this.mapContainer = new JScrollPane();
        this.mapPanel     = new LayoutManagerMapPanel(this);
        this.reservationDetailPanel = new ReservationDetailPanel(this);

        this.insertPointButton = new JButton("Insert Point");
        this.insertLineButton= new JButton("Insert Line");

        this.showButton = new JButton("Show");
        this.showButton.addActionListener(new LmpShowButtonListener(this));

        this.insertButton = new JButton("Create Reservation");
        this.insertButton.addActionListener(new LmpInsertButtonListener(this));

        this.updateButton = new JButton("Update Reservation");
        this.updateButton.addActionListener(new LmpUpdateButtonListener(this));

        this.saveButton = new JButton(("Save All"));
        this.btnsEnabled  = true; // Then changed to false

        this.toolPanel.setLayout(new BoxLayout(this.toolPanel, BoxLayout.Y_AXIS));
        this.gbc.fill    = GridBagConstraints.BOTH;
        this.gbc.gridx   = 0;
        this.gbc.gridy   = 0;
        this.gbc.weightx = 1;
        this.gbc.weighty = 1;
        this.add(this.toolPanel, this.gbc);
        this.buildTools();

        this.calendarPanel.add(reservationsLabel);
        this.calendarPanel.add(stagesLabel);
        this.calendarPanel.add(calendarDate);
        this.calendarPanel.add(showButton);
        this.calendarPanel.setLayout(new BoxLayout(
            this.calendarPanel, BoxLayout.X_AXIS));
        this.gbc.anchor    = GridBagConstraints.NORTH;
        this.gbc.fill      = GridBagConstraints.HORIZONTAL;
        this.gbc.gridx     = 1;
        this.gbc.gridy     = 1;
        this.gbc.weightx   = 1;
        this.gbc.weighty   = 1;
        this.add(this.calendarPanel, this.gbc);

        this.mapContainer.setHorizontalScrollBarPolicy(
                JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        this.mapContainer.setVerticalScrollBarPolicy(
                JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        this.mapContainer.setViewportView(this.mapPanel);

        this.gbc.anchor    = GridBagConstraints.NORTH;
        this.gbc.fill      = GridBagConstraints.BOTH;
        this.gbc.gridx     = 1;
        this.gbc.gridy     = 0;
        this.gbc.weightx   = 6;
        this.gbc.weighty   = 99;
        this.add(this.mapContainer, this.gbc);

        this.reservationDetailPanel.setLayout(new BoxLayout(
            this.reservationDetailPanel, BoxLayout.Y_AXIS));
        this.gbc.fill      = GridBagConstraints.HORIZONTAL;
        this.gbc.anchor      = GridBagConstraints.NORTH;
        this.gbc.gridx   = 2;
        this.gbc.gridy   = 0;
        this.gbc.weightx = 1;
        this.gbc.weighty = 1;
        this.add(this.reservationDetailPanel, this.gbc);

        this.updateDetails();
    }

    /**
     * Build tool panel.
     */
    private void buildTools()
    {
        this.buildRadioButtons();
        this.buildButtons();
        this.buildDetails();
    }

    /**
     * Build radio buttons.
     */
    private void  buildRadioButtons()
    {
        this.pRBtns = new JPanel();
        this.pRBtns.setLayout(new GridLayout(3, 1));

        Border selectModeBorder = BorderFactory.createTitledBorder("Select shape: ");
        pRBtns.setBorder(selectModeBorder);

        this.modeShapeSelect = new JRadioButton("Select");
        this.modeShapeMove   = new JRadioButton("Move");
        this.modeShapeResize = new JRadioButton("Resize");
        this.selectMode      = new ButtonGroup();

        this.selectMode.add(this.modeShapeSelect);
        this.selectMode.add(this.modeShapeMove);
        this.selectMode.add(this.modeShapeResize);

        this.pRBtns.add(this.modeShapeSelect);
        this.pRBtns.add(this.modeShapeMove);
        this.pRBtns.add(this.modeShapeResize);

        this.modeShapeSelect.setSelected(true);


        this.modeShapeMove.addItemListener(this);
        this.modeShapeResize.addItemListener(this);
        this.modeShapeSelect.addItemListener(this);

        this.toolPanel.add(this.pRBtns);
    }

    /**
     * Build buttons.
     */
    private void buildButtons()
    {
        this.pBtns = new JPanel(new GridLayout(6, 1));
        this.gbc.gridx   = 1;
        this.gbc.gridy   = 0;
        this.delBtn = new JButton("Delete");
        this.insertLineButton.addActionListener(this);
        this.insertPointButton.addActionListener(this);
        this.delBtn.addActionListener(this);
        this.saveButton.addActionListener(this);
        this.delBtn.setEnabled(false);
        this.updateButton.setEnabled(false);

        this.pBtns.add(this.insertPointButton, this.gbc);
        this.pBtns.add(this.insertLineButton, this.gbc);
        this.pBtns.add(this.insertButton, this.gbc);
        this.pBtns.add(this.updateButton,this.gbc);
        this.pBtns.add(this.delBtn, this.gbc);
        this.pBtns.add(this.saveButton, this.gbc);

        this.toolPanel.add(this.pBtns);
    }

    /**
     * Build table showing shape details.
     */
    private void buildDetails()
    {
        this.pDetails    = new JPanel();
        this.pDetails.setLayout(new GridBagLayout());

        this.tDetails = new JTable(6, 2);
        this.tDetails.setValueAt("Name",      0, 0);
        this.tDetails.setValueAt("Area",      1, 0);
        this.tDetails.setValueAt("Length",    2, 0);
        this.tDetails.setValueAt("Biggest",   3, 0);
        this.tDetails.setValueAt("Nearest",   4, 0);
        this.tDetails.setValueAt("Has point", 5, 0);

        this.pDetails.add(this.tDetails);
        this.toolPanel.add(this.pDetails);
    }
    
    /**
     * Gets map panel child.
     * @return map panel
     */
    public LayoutManagerMapPanel getMapPanel()
    {
        return this.mapPanel;
    }
    
    /**
     * Unselect all.
     */
    public void unselectedAll()
    {
        this.selectMode.clearSelection();
    }
    
    /**
     * Gets date from calendar/
     * @return date
     */
    public Date getDate()
    {
        return this.calendarDate.getDate();
    }

    /**
     * Item changed event handler for radio buttons.
     * @param e event data
     */
    @Override
    public void itemStateChanged(ItemEvent e)
    {
        JRadioButton item = (JRadioButton) e.getItem();
        if(item.isSelected())
        {
            LmMode selectedMode = LmMode.SELECT;
            this.reservationDetailPanel.showInsertOptions(false);
            if("Insert".equals(item.getText())) {
                selectedMode = LmMode.INSERT;
            }
            else if("Move".equals(item.getText()))
                selectedMode = LmMode.MOVE;
            else if("Resize".equals(item.getText()))
                selectedMode = LmMode.RESIZE;
            else if("Select".equals(item.getText()))
                selectedMode = LmMode.SELECT;

            this.mapPanel.setSelectedMode(selectedMode);
        }
    }

    /**
     * Gets selected type by button name.
     * @param btnName button name
     * @return selected shape type
     */
    public ShapeType getSelectedShapeByButtonName(String btnName)
    {
        if("Point".equals(btnName))
            return ShapeType.POINT;
        else if("Line".equals(btnName))
            return ShapeType.LINE;
        else if("Rectangle".equals(btnName))
            return ShapeType.RECTANGLE;
        else if("Circle".equals(btnName))
            return ShapeType.CIRCLE;
        else if("Polygon".equals(btnName))
            return ShapeType.POLYGON;
        else
            return ShapeType.NONE;
    }

    /**
     * Action performed event handler for radio buttons
     * @param e ActionEvent
     */
    @Override
    public void actionPerformed(ActionEvent e)
    {
        JButton src = (JButton) e.getSource();
        if("Save All".equals(src.getText()))
            this.mapPanel.saveAll();
        else if("Delete".equals(src.getText()))
            this.mapPanel.deleteSelected();
        else if("Insert Point".equals(src.getText()))
        {
            this.mapPanel.setSelectedShapeType(ShapeType.POINT);
            this.mapPanel.setSelectedMode(LmMode.INSERT);
            this.unselectedAll();
        }
        else if("Insert Line".equals(src.getText()))
        {
            this.mapPanel.setSelectedShapeType(ShapeType.LINE);
            this.mapPanel.setSelectedMode(LmMode.INSERT);
            this.unselectedAll();
        }
    }

    /**
     * Enable or disable delete button. Depends on currenlty selected shape.
     * @param enabled value to set
     */
    public void setEnabledDeleteButton(boolean enabled)
    {
        if(enabled && this.modeShapeSelect.isSelected()){
            this.delBtn.setEnabled(enabled);
            ShapeType type = mapPanel.getCurrentSelection().getType();
            if(type == ShapeType.RECTANGLE || type == ShapeType.CIRCLE ||
            type == ShapeType.POLYGON || type == ShapeType.COMPOSITE)
            {
                this.updateButton.setEnabled(enabled);
            }
            else
            {
                this.updateButton.setEnabled(false);
            }
        }
        else{
            this.delBtn.setEnabled(false);
            this.updateButton.setEnabled(false);
        }
    }

    /**
     * Update shape details table.
     */
    public void updateDetails()
    {
        ShapeBase s = this.mapPanel.getCurrentSelection();
        if(s == null)
        {
            this.tDetails.setValueAt("", 0, 1);
            this.tDetails.setValueAt("", 1, 1);
            this.tDetails.setValueAt("", 2, 1);
            this.tDetails.setValueAt("", 3, 1);
            this.tDetails.setValueAt("", 4, 1);
            this.tDetails.setValueAt("", 5, 1);
            this.reservationDetailPanel.showNothing();
        }
        else
        {

            this.tDetails.setValueAt(s.getName(), 0, 1);

            double x = LayoutManagerModel.getAreaofShape(s);
            this.tDetails.setValueAt(x, 1, 1);
            x = LayoutManagerModel.getLengthofShape(s);
            this.tDetails.setValueAt(x, 2, 1);

            ShapeBase sb = LayoutManagerModel.getBiggestShape();
            String st = sb == null ? "N/A" : sb.getName();
            this.tDetails.setValueAt(st, 3, 1);
            sb = LayoutManagerModel.getNearestShape(s);
            st = sb == null ? "N/A" : sb.getName();
            this.tDetails.setValueAt(st, 4, 1);

            boolean o = LayoutManagerModel.isOverlappedByMultipoint(s);
            this.tDetails.setValueAt(Boolean.toString(o), 5, 1);
            if(s.isOwnable()) {
                this.reservationDetailPanel.updateReservationDetailPanel(s.reservation);
            }
            else{
                this.reservationDetailPanel.showNothing();
            }
        }
    }
    
    // Unused document events
    
    @Override
    public void insertUpdate(DocumentEvent e) {}

    @Override
    public void removeUpdate(DocumentEvent e) {}

    @Override
    public void changedUpdate(DocumentEvent e) {}
}
