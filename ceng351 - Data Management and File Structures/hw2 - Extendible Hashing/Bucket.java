package ceng.ceng351.labdb;


import java.util.ArrayList;

public class Bucket {

    private int numberOfRecords;
    private int localDepth;
    private ArrayList<String> recordsTable;

    public Bucket(int bucketSize, int localDepth){
        this.localDepth = localDepth;
        this.recordsTable = new ArrayList<String>(bucketSize);
        this.numberOfRecords = 0;
    }

    public void setRecordsTable(ArrayList<String> recordsTable) {
        this.recordsTable = recordsTable;
        this.numberOfRecords = recordsTable.size();
    }

    public ArrayList<String> getRecordsTable() {
        return recordsTable;
    }

    public void increaseLocalDepthByOne() {
        this.localDepth ++;
    }

    public void decreaseLocalDepthByOne() {
        this.localDepth --;
    }

    public int getLocalDepth() {
        return localDepth;
    }

    public int getNumberOfRecords() {
        return numberOfRecords;
    }

    public void removeRecord(String record) {
        recordsTable.remove(record);
        this.numberOfRecords --;
    }

    public void removeAllRecord() {
        recordsTable.clear();
        this.numberOfRecords= 0;
    }

    public void setNumberOfRecords(int numberOfRecords) {
        this.numberOfRecords = numberOfRecords;
    }

    public void addRecordsTable(String record) {
        this.numberOfRecords ++;
        this.recordsTable.add(record);
    }

    public void printBucket(){
        System.out.print("[Local depth:" + this.localDepth + "]");
        for (int i = 0; i < numberOfRecords; i++){
            System.out.print("<"+recordsTable.get(i)+">");
        }
        System.out.print("\n");
    }
}
