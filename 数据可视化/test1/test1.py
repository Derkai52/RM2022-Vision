import numpy as np
import matplotlib
import matplotlib.mlab as mlab

from matplotlib.animation import FuncAnimation   #导入负责绘制动画的接口
import matplotlib.font_manager as fm
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
print(matplotlib.matplotlib_fname())

plt.rcParams['font.sans-serif']=['SimHei'] #用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False #用来正常显示负号
# zhfont= matplotlib.font_manager.FontProperties(fname='/usr/share/fonts/truetype/arphic/uming.ttc')
def simple_plot():
    """
    simple plot
    """
    # 生成测试数据
    x = np.linspace(-np.pi, np.pi, 256, endpoint=True)
    y_cos, y_sin = np.cos(x), np.sin(x)

    # 生成画布，并设定标题
    plt.figure(figsize=(8, 6), dpi=80)
    plt.title("简单曲线图")
    plt.grid(True)

    # 设置X轴
    plt.xlabel("科目")
    plt.xlim(-4.0, 4.0)
    plt.xticks(np.linspace(-4, 4, 9, endpoint=True))

    # 设置Y轴
    plt.ylabel("成绩")
    plt.ylim(0, 100)
    plt.yticks(np.linspace(0, 100, 10, endpoint=True))

    # 画两条曲线
    plt.plot(x, y_cos, "b--", linewidth=2.0, label="cos示例")
    plt.plot(x, y_sin, "g-", linewidth=2.0, label="sin示例")

    # 设置图例位置,loc可以为[upper, lower, left, right, center]
    plt.legend(loc="upper left", shadow=True)

    # 图形显示
    plt.show()
    return
# simple_plot()

def bar_plot():
    """
    bar plot
    """


    # 生成测试数据
    means_men = (60, 61, 85, 85, 85, 30)
    means_women = (88, 78, 95, 86, 80, 85)

    # 设置标题
    plt.figure(figsize=(10, 6), dpi=150)
    plt.title("19063227唐凯")

    # 设置相关参数
    index = np.arange(len(means_men))
    bar_width = 0.35

    # 画柱状图
    plt.bar(index, means_men, width=bar_width, alpha=0.2, color="b", label="废物的我")
    plt.bar(index+bar_width, means_women, width=bar_width, alpha=0.8, color="r", label="学霸朋友")
    # 画折线图
    # plt.plot(index, means_men, "b-", linewidth=2.0, label="废物的我")
    # plt.plot(index+bar_width, means_women, "r--", linewidth=2.0, label="学霸朋友")
    plt.legend(loc="upper right", shadow=True)

    # 设置柱状图标示
    for x, y in zip(index, means_men):
        plt.text(x, y+0.3, y, ha="center", va="bottom")
    for x, y in zip(index, means_women):
        plt.text(x+bar_width, y+0.3, y, ha="center", va="bottom")

    # 设置刻度范围/坐标轴名称等
    plt.ylim(0, 100)
    plt.xlabel("科目")
    plt.ylabel("期末成绩")
    plt.xticks(index+(bar_width/2), ("大数据领域与建模", "机器学习与模式识别", "软件工程", "数据挖掘与清洗", "计算机组成原理", "海量数据存储与运维"))

    # 图形显示
    plt.savefig("./test2.jpg")
    plt.show()
    return
bar_plot()