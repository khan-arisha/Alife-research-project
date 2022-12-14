from iterpop import iterpop as ip
from keyname import keyname as kn
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

try:
    __, synchronous = sys.argv
except:
    raise ValueError('specify "synchronous" as argument')

try:
    synchronous = int(synchronous)
except:
    raise ValueError('"synchronous" should be an integer')

assert synchronous in (0, 1), '"synchronous" should be 0 or 1'

# load and set up DataFrame
actual = pd.read_csv(kn.pack({
  "synchronous" : str(synchronous),
  "ext" : ".csv",
}))
actual["Type"] = "Actual"

# get ready to calculate ideal timings in terms actual timings
# with thread count 1
ideal = actual.copy()
ideal["Type"] = "Ideal"

# helper functions
get_baseline_idx = lambda row: actual[
    (actual["Work"] == row["Work"])
    & (actual["Replicate"] == row["Replicate"])
]["Threads"].idxmin()

get_baseline_time = lambda row: actual.iloc[
    get_baseline_idx(row)
]["Time"]

get_baseline_threads = lambda row: actual.iloc[
    get_baseline_idx(row)
]["Threads"]

# calculate ideal time in terms of lowest-threaded observation
ideal["Time"] = ideal.apply(
    lambda row: (
        get_baseline_time(row)
        * get_baseline_threads(row)
        / row["Threads"]
    ),
    axis=1,
)

# drop thread counts from ideal that were defined identically to actual
ideal = ideal.drop(ideal.index[
    ideal.apply(
        lambda row: row["Threads"] == ideal[
            ideal["Work"] == row["Work"]
        ]["Threads"].min(),
        axis=1,
    ),
])

# merge in newly-calculated ideal timings
df = pd.concat([ideal, actual])

# draw plot
g = sns.FacetGrid(
  df,
  col="Work",
  col_wrap=3,
  margin_titles="true",
  sharey=False,
)

g.map(
  sns.barplot,
  "Threads",
  "Time",
  "Type",
  hue_order=sorted(df["Type"].unique()),
  order=sorted(df["Threads"].unique()),
  palette=sns.color_palette(),
).add_legend()

plt.savefig(
  kn.pack({
    "title" : "strong-scaling",
    "synchronous" : str(synchronous),
    "ext" : ".png",
  }),
  transparent=True,
  dpi=300,
)
