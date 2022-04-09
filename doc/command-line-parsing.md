# 解析命令行选项

使用命令行选项 (Command-Line Option) 来控制程序的执行是很常见的操作, 这可以使程序变得更加灵活——仅通过修改调用时命令行的选项即可控制程序的行为. 

> 有时命令行选项也称命令行参数 (Command-Line Arguments / Args).

一个常见的例子是, 在 Shell 程序中, 可以通过修改调用时的参数, 来使程序读取不同的文件:

Unix-like:

```console
$ ./bin --from-video='/usr/henry/example.mp4'
```

Windows:

```console
PS> ./bin --from-video='C:/Users/Henry/example.mp4'
```

OpenCV 提供了一个较为简单的命令行解析器实现, 它只能解析整个的命令行选项的字符串, 如 `-h`, `--use-camera`; 或者带有值的参数, 如 `--key=value`.

一般, 命令行选项会通过 Shell 传递给程序, 因此应确保这些键值对字符串被 Shell 程序视为一个整体. 

例如, 键入命令 `-a=0.5` 时, 需要注意里面没有空格. 在 PowerShell 中, 甚至需要将其写为 `-a='0.5'`. 

此外, 如果要通过命令行选项传递字符串, 应确保将字符串作为一个整体 (尤其是字符串中有空格时), 通常会将其括在一对引号中. 例如 `--out-file='/home/henry/save/result.mp4'`.

> 如果是通过 IDE 等程序进行调试, 亦需确保每个参数选项是作为一个整体传递的, 而是否需要进行转义等处理应视具体情况而定.

详情可以阅读 OpenCV 关于 [`cv::CommandLineParser` 的官方文档][parser-doc], 以及 GitHub 上 OpenCV (4.x) 仓库中的源码:

- [头文件](https://github.com/opencv/opencv/blob/4.x/modules/core/include/opencv2/core/utility.hpp#L817)
- [实现源代码](https://github.com/opencv/opencv/blob/4.x/modules/core/src/command_line_parser.cpp)
- [单元测试](https://github.com/opencv/opencv/blob/4.x/modules/core/test/test_utils.cpp)

[parser-doc]: https://docs.opencv.org/4.x/d0/d2e/classcv_1_1CommandLineParser.html

自然, 也可以使用其他库提供的命令行选项解析器实现. 这里考虑到已经使用了 OpenCV 库的其他功能, 并且 OpenCV 库中的提供的实现使用起来较为简便, 于是选择了该实现. 项目中提供了一个简单的 [Demo](../cli_parser_demo.cpp) 以供参考. 

<!-- Some English Version

    Arguments with default values are considered to be always present.

    OpenCV provides a rather simple command-line parser implementation, which 
    only parse whole strings of options in a form similar to `-h`, `--use-camera`
    or options with value like `--key=value`.
    
    Command line options are often passed to the programs through shells,
    so the user should ensure that those 'key-value' pair strings are regarded
    as a whole part by the shell program.

    For example, `-a=0.5` should be written with no spaces inside. In PowerShell,
    one may even need to type this as `-a='0.5'`.

    Also, if one is going to pass a string via command-line options, they should
    ensure that the string is taken as a whole part, usually by enclosing it in 
    a pair of quotes. e.g. `--out-file='/home/henry/save/result.mp4'`.
-->

参考使用:

> TIP: If OpenCV was built with Qt, and the program runs on High-DPI displays, 
> set enviroment variable `QT_AUTO_SCREEN_SCALE_FACTOR` to the value of `1`:
> 
> PowerShell:
> 
> ```console
> PS> $Env:QT_AUTO_SCREEN_SCALE_FACTOR=1
> ```

```console
$ ./bin --mode='abc'
$ ./bin --mode='abc' --from-file --file-path='/home/henry/example.mp4'
$ ./bin --mode='abc' --from-usbcam --cam-id=0
$ ./bin --mode='abc' --from-usbcam --cam-id=0 --visual
$ ./bin --mode='abc' --from-usbcam --cam-id=0 --visual --stepping
$ ./bin --mode='abc' --from-usbcam --cam-id=0 --visual --fps=30 --write-input --write-result
```
