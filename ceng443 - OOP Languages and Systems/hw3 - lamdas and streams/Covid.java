import java.util.*;
import java.util.Map;
import java.util.stream.Collectors;
import java.nio.file.Paths;
import java.util.Arrays;
import java.nio.file.Files;
import java.util.stream.Stream;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.function.Predicate;
import java.util.function.Function;

public class Covid
{
  // You can add your own variables between them.

  // You can add your own variables between them.

  // You must not change between them.
  private List<List<String>> rows;

  public Covid()
  {
    try
    {
      this.rows = Files
  				.lines(Paths.get("covid19.csv"))
  				.map(row -> Arrays.asList(row.split(",")))
  				.collect(Collectors.toList());
    }
    catch (IOException e)
    {
			e.printStackTrace();
		}
  }
  // You must not change between them.

  public void printOnlyCases(String location, String date)
  {
    Integer result =
            rows.stream().filter(row -> row.get(1).equals(location))
                              .filter(row -> row.get(2).equals(date))
                              .mapToInt(row -> Integer.parseInt(row.get(3))-Integer.parseInt(row.get(5)))
                              .sum();

    System.out.printf("Result: " + result);
  }

  public long getDateCount(String location)
  {
    long toReturn = rows.stream().filter(row -> row.get(1).equals(location)).count();
    return toReturn;
  }

  public int getCaseSum(String date)
  {
    int toReturn = rows.stream()
                       .filter(row -> row.get(2).equals(date))
                       .mapToInt(row -> Integer.parseInt(row.get(4)))
                       .sum();

    return toReturn;
  }

  private Predicate<List<String>> checkZeroRowsCount =
          row -> { return(Integer.parseInt(row.get(3)) == 0 &&
                          Integer.parseInt(row.get(4)) == 0 &&
                          Integer.parseInt(row.get(5)) == 0 &&
                          Integer.parseInt(row.get(6)) == 0);
                  };

  public long getZeroRowsCount(String location)
  {
    long toReturn = rows.stream()
                        .filter(row -> row.get(1).equals(location))
                        .filter(checkZeroRowsCount)
                        .count();
    return toReturn;
  }

  public double getAverageDeath(String location)
  {
    double toReturn = rows.stream()
                          .filter(row -> row.get(1).equals(location))
                          .mapToInt(row -> Integer.parseInt(row.get(6)))
                          .average()
                          .orElse(-1);

    toReturn = Double.parseDouble(new DecimalFormat("##.00").format(toReturn));
    return toReturn;
  }

  public String getFirstDeathDayInFirstTenRows(String location)
  {
    String toReturn = rows.stream()
                          .filter(row -> row.get(1).equals(location))
                          .limit(10)
                          .filter(row -> Integer.parseInt(row.get(5)) != 0)
                          .filter(row -> row.get(5).equals(row.get(6)) )
                          .findFirst()
                          .map(row -> row.get(2))
                          .orElse("Not Found");
    return toReturn;
  }

  private Function<List<String>,List<String>> firstTwo =
          n -> { List<String> list = Arrays.asList(n.get(0), n.get(1));
                  return Arrays.asList(n.get(0), n.get(1));
                };


  public String[] getDateCountOfAllLocations()
  {
    String[] toReturn = rows.stream()
                            .map(firstTwo)
                            .distinct()
                            .map(e -> e.get(0) + ": " + this.getDateCount(e.get(1)))
                            .sorted()
                            .toArray(String[]::new);
    return toReturn;
  }

  private String getFirstDeathDay(String location){
    String toReturn = rows.stream()
            .filter(row -> row.get(1).equals(location))
            .filter(row -> Integer.parseInt(row.get(5)) != 0)
            .filter(row -> row.get(5).equals(row.get(6)) )
            .findFirst()
            .map(row -> row.get(2))
            .orElse("Not Found");
    return toReturn;
  }

  public List<String> getLocationsFirstDeathDay()
  {
    List<String> toReturn = this.rows.stream()
                                      .map(firstTwo)
                                      .distinct()
                                      .filter(e->!this.getFirstDeathDay(e.get(1)).equals("Not Found"))
                                      .map(e -> e.get(1) + ": " + this.getFirstDeathDay(e.get(1)))
                                      .sorted()
                                      .collect(Collectors.toList());
    return toReturn;
  }

  public String trimAndGetMax(String location, int trimCount)
  {
    String toReturn = null;
    List<String> res = this.rows.stream()
            .filter(row -> row.get(1).equals(location))
            .sorted(Comparator.comparing(e->Integer.parseInt(e.get(4))))
            .skip(trimCount)
            .limit(this.getDateCount(location) - (2*trimCount))
            .max(Comparator.comparing(e->Integer.parseInt(e.get(6))))
            .get();
    return res.get(2)+ ": " + res.get(6);
  }

  public List<List<String>> getOnlyCaseUpDays(String location)
  {
    List<List<String>> toReturn = this.rows.stream()
                                            .filter(row -> row.get(1).equals(location))
                                            .filter(row -> Integer.parseInt(row.get(4)) != 0)
                                            .collect(Collectors.toList());
    System.out.printf("Result: " + toReturn.size());
    return toReturn;
  }

  public static void main(String[] args)
  {
    // You can test your function in here.
    // This part is not graded.
    Covid covid = new Covid();
    System.out.println("*****************1");
    covid.printOnlyCases("United States", "2020-02-25");

    System.out.println("*****************2");
    System.out.println(covid.getDateCount("Turkey"));

    System.out.println("*****************3");
    System.out.println(covid.getCaseSum("2020-04-05"));

    System.out.println("*****************4");
    System.out.println(covid.getZeroRowsCount("Turkey"));
    System.out.println(covid.getZeroRowsCount("Australia"));

    System.out.println("*****************5");
    System.out.println(covid.getAverageDeath("Turkey"));

    System.out.println("*****************6");
    System.out.println(covid.getFirstDeathDayInFirstTenRows("Turkey"));
    System.out.println(covid.getFirstDeathDayInFirstTenRows("Italy"));

    System.out.println("*****************7");
    System.out.println(Arrays.toString(covid.getDateCountOfAllLocations()));

    System.out.println("*****************8");
    System.out.println(Arrays.toString(covid.getLocationsFirstDeathDay().toArray()));

    System.out.println("*****************9");
    System.out.println(covid.trimAndGetMax("Turkey", 5));

    System.out.println("*****************10");
    System.out.println(covid.getOnlyCaseUpDays("Turkey"));
  }
}
